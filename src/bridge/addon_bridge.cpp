#include <napi.h>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "api/nesting_api.hpp"
#include "geometry/geometry_types.hpp"
#include "nesting/nesting_result.hpp"
#include "pattern/pattern_piece.hpp"
#include "pattern/shapes.hpp"
#include "pattern/variant_constraint.hpp"
#include "utils/logger_setup.hpp"

namespace {

using pmg::geometry::Point64;
using pmg::pattern::Fabric;
using pmg::pattern::PatternPiece;
using pmg::pattern::VariantConstraint;

int64_t getInt(const Napi::Object& obj, const char* key, int64_t def) {
    if (!obj.Has(key)) return def;
    Napi::Value v = obj.Get(key);
    if (v.IsUndefined() || v.IsNull()) return def;
    if (!v.IsNumber()) throw std::invalid_argument(std::string("'") + key + "' must be a number");
    return v.As<Napi::Number>().Int64Value();
}

bool getBool(const Napi::Object& obj, const char* key, bool def) {
    if (!obj.Has(key)) return def;
    Napi::Value v = obj.Get(key);
    if (v.IsUndefined() || v.IsNull()) return def;
    if (!v.IsBoolean()) throw std::invalid_argument(std::string("'") + key + "' must be a boolean");
    return v.As<Napi::Boolean>().Value();
}

Napi::Array requireArray(const Napi::Object& obj, const char* key) {
    Napi::Value v = obj.Get(key);
    if (!v.IsArray()) throw std::invalid_argument(std::string("'") + key + "' must be an array");
    return v.As<Napi::Array>();
}

std::vector<Point64> parsePoints(const Napi::Array& arr) {
    std::vector<Point64> result;
    result.reserve(arr.Length());
    for (uint32_t i = 0; i < arr.Length(); ++i) {
        Napi::Value item = arr.Get(i);
        if (!item.IsObject()) throw std::invalid_argument("point must be an object {x, y}");
        Napi::Object o = item.As<Napi::Object>();
        result.push_back(Point64{getInt(o, "x", 0), getInt(o, "y", 0)});
    }
    return result;
}

std::vector<VariantConstraint> parseConstraints(const Napi::Array& arr) {
    std::vector<VariantConstraint> result;
    result.reserve(arr.Length());
    for (uint32_t i = 0; i < arr.Length(); ++i) {
        Napi::Value item = arr.Get(i);
        if (!item.IsObject()) throw std::invalid_argument("constraint must be an object");
        Napi::Object o = item.As<Napi::Object>();
        VariantConstraint c{};
        c.rotationAngle = static_cast<int>(getInt(o, "rotationAngle", 0));
        c.isMirrored = getBool(o, "isMirrored", false);
        c.requiredQuantity = static_cast<int>(getInt(o, "requiredQuantity", 1));
        c.twoWay = getBool(o, "twoWay", false);
        result.push_back(c);
    }
    return result;
}

PatternPiece parsePiece(const Napi::Object& o) {
    std::vector<Point64> points = parsePoints(requireArray(o, "points"));
    std::vector<VariantConstraint> constraints;
    if (o.Has("constraints")) constraints = parseConstraints(requireArray(o, "constraints"));
    return PatternPiece(getInt(o, "id", 0), std::move(points), constraints,
                        getInt(o, "seamAllowance", 0), getInt(o, "cuttingAllowance", 0));
}

Fabric parseFabric(const Napi::Object& o) {
    if (o.Has("boundary")) return Fabric(parsePoints(requireArray(o, "boundary")));
    if (o.Has("width") && o.Has("length")) return Fabric(getInt(o, "width", 0), getInt(o, "length", 0));
    throw std::invalid_argument("fabric requires {width, length} or {boundary: [{x, y}, ...]}");
}

struct NestingInput {
    Fabric fabric;
    std::vector<PatternPiece> pieces;
};

NestingInput parseInput(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsArray())
        throw std::invalid_argument("usage: execute(fabric: object, pieces: object[])");

    NestingInput in{parseFabric(info[0].As<Napi::Object>()), {}};
    Napi::Array arr = info[1].As<Napi::Array>();
    in.pieces.reserve(arr.Length());
    for (uint32_t i = 0; i < arr.Length(); ++i) {
        Napi::Value v = arr.Get(i);
        if (!v.IsObject()) throw std::invalid_argument("each piece must be an object");
        in.pieces.push_back(parsePiece(v.As<Napi::Object>()));
    }
    return in;
}

Napi::Object toJs(Napi::Env env, const pmg::nesting::NestingResult& r) {
    Napi::Object out = Napi::Object::New(env);
    out.Set("wasSuccessful", r.wasSuccessful);
    out.Set("minLength", r.minLength);
    out.Set("executionTime", r.executionTime);

    Napi::Object stats = Napi::Object::New(env);
    stats.Set("usableScrapCount", r.stats.usableScrapCount);
    stats.Set("usableScrapsArea", r.stats.usableScrapsArea);
    stats.Set("usableScrapsPercentage", r.stats.usableScrapsPercentage);
    stats.Set("remainingFabricLength", r.stats.remainingFabricLength);
    stats.Set("remainingFabricArea", r.stats.remainingFabricArea);
    stats.Set("remainingFabricPercentage", r.stats.remainingFabricPercentage);
    stats.Set("wasteArea", r.stats.wasteArea);
    stats.Set("wastePercentage", r.stats.wastePercentage);
    out.Set("stats", stats);

    Napi::Array positions = Napi::Array::New(env, r.positions.size());
    for (size_t i = 0; i < r.positions.size(); ++i) {
        const auto& p = r.positions[i];
        Napi::Object o = Napi::Object::New(env);
        o.Set("id", Napi::Number::New(env, static_cast<double>(p.id)));
        o.Set("rotationAngle", p.rotationAngle);
        o.Set("isMirrored", p.isMirrored);
        o.Set("isFlipped180", p.isFlipped180);
        o.Set("x", Napi::Number::New(env, static_cast<double>(p.x)));
        o.Set("y", Napi::Number::New(env, static_cast<double>(p.y)));
        positions[static_cast<uint32_t>(i)] = o;
    }
    out.Set("positions", positions);
    return out;
}

class ExecuteWorker : public Napi::AsyncWorker {
public:
    ExecuteWorker(Napi::Env env, NestingInput input)
        : Napi::AsyncWorker(env), m_deferred(Napi::Promise::Deferred::New(env)), m_input(std::move(input)) {}

    Napi::Promise Promise() { return m_deferred.Promise(); }

protected:
    void Execute() override {
        try {
            NestingEngineFacade facade(m_input.fabric);
            m_result = facade.execute(m_input.pieces);
        } catch (const std::exception& e) {
            SetError(e.what());
        }
    }
    void OnOK() override { m_deferred.Resolve(toJs(Env(), m_result)); }
    void OnError(const Napi::Error& e) override { m_deferred.Reject(e.Value()); }

private:
    Napi::Promise::Deferred m_deferred;
    NestingInput m_input;
    pmg::nesting::NestingResult m_result{};
};

Napi::Value Execute(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    try {
        auto* worker = new ExecuteWorker(env, parseInput(info));
        Napi::Promise p = worker->Promise();
        worker->Queue();
        return p;
    } catch (const std::exception& e) {
        Napi::Promise::Deferred d = Napi::Promise::Deferred::New(env);
        d.Reject(Napi::Error::New(env, e.what()).Value());
        return d.Promise();
    }
}

Napi::Value ExecuteSync(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    try {
        NestingInput in = parseInput(info);
        NestingEngineFacade facade(in.fabric);
        return toJs(env, facade.execute(in.pieces));
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

} // namespace

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    try { pmg::logging::initialize(); } catch (const std::exception&) {}
    exports.Set("execute", Napi::Function::New(env, Execute, "execute"));
    exports.Set("executeSync", Napi::Function::New(env, ExecuteSync, "executeSync"));
    return exports;
}

NODE_API_MODULE(pmg_engine, Init)
