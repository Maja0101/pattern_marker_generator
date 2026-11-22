#include <spdlog/spdlog.h>

#include "sample_input.hpp"
#include "common/geometry_types.hpp"
#include "pattern/pattern_piece.hpp"

namespace pmg::samples {

std::vector<pattern::PatternPiece> getPiecesToCheck()
{
    int sa = 5;
    int ca = 20;
    int g = 0;

    std::vector<pattern::PatternPiece> patternPieces;
    patternPieces.reserve(8);

    std::vector<geometry::Point64> shape1 = {
        {285, 202},
        {297, 14},
        {467, 13},
        {488, 202}
    };

    std::vector<pattern::VariantConstraint> constraints1 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(1, shape1, constraints1, sa, ca, 0, true);

    std::vector<geometry::Point64> shape2 = {
        {158, 497},
        {175, 337},
        {229, 309},
        {250, 288},
        {266, 261},
        {286, 266},
        {278, 329},
        {284, 417},
        {248, 423},
        {224, 443},
        {211, 466},
        {206, 497}
    };

    std::vector<pattern::VariantConstraint> constraints2 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 2, .twoWay = false }
    };

    patternPieces.emplace_back(2, shape2, constraints2, sa, ca, g, true);

    std::vector<geometry::Point64> shape3 = {
        {152, 499},
        {115, 476},
        {58, 452},
        {59, 390},
        {68, 297},
        {168, 328},
        {162, 432},
        {156, 488}
    };

    std::vector<pattern::VariantConstraint> constraints3 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(3, shape3, constraints3, sa, ca, g, true);

    std::vector<geometry::Point64> shape4 = {
        {614, 508},
        {591, 506},
        {556, 497},
        {554, 458},
        {542, 431},
        {520, 412},
        {493, 405},
        {513, 323},
        {513, 256},
        {517, 252},
        {536, 250},
        {560, 298},
        {581, 319},
        {614, 340},
        {605, 506},
        {611, 506},
        {614, 497}
    };

    std::vector<pattern::VariantConstraint> constraints4 = {
        { .rotationAngle = 90, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(4, shape4, constraints4, sa, ca, g, true);

    std::vector<geometry::Point64> shape5 = {
        {610, 513},
        {611, 458},
        {622, 337},
        {624, 334},
        {732, 322},
        {716, 478},
        {711, 482},
        {686, 485}
    };

    std::vector<pattern::VariantConstraint> constraints5 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(5, shape5, constraints5, sa, ca, g, true);

    std::vector<geometry::Point64> shape6 = {
        {397, 453},
        {382, 451},
        {367, 436},
        {351, 427},
        {356, 356},
        {351, 237},
        {370, 229},
        {410, 230},
        {423, 238},
        {422, 345},
        {429, 429},
        {413, 438}
    };

    std::vector<pattern::VariantConstraint> constraints6 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(6, shape6, constraints6, sa, ca, g, true);

    std::vector<geometry::Point64> shape7 = {
        {439, 436},
        {433, 434},
        {435, 310},
        {429, 257},
        {429, 226},
        {451, 233},
        {477, 251},
        {487, 267},
        {481, 343},
        {488, 405},
        {487, 422},
        {456, 426}
    };

    std::vector<pattern::VariantConstraint> constraints7 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(7, shape7, constraints7, sa, ca, g, true);

    std::vector<geometry::Point64> shape8 = {
        {347, 435},
        {317, 423},
        {295, 423},
        {291, 418},
        {296, 347},
        {294, 308},
        {287, 267},
        {297, 251},
        {310, 239},
        {340, 226},
        {346, 226},
        {342, 326}
    };

    std::vector<pattern::VariantConstraint> constraints8 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(8, shape8, constraints8, sa, ca, g, true);

    return patternPieces;
}

std::vector<pattern::PatternPiece> getPiecesToCheck2()
{
    int sa = 5;
    int ca = 20;
    int g = 0;

    std::vector<pattern::PatternPiece> patternPieces;
    patternPieces.reserve(6);

    std::vector<geometry::Point64> shape7 = {
        {109, 321},
        {34, 312},
        {30, 10},
        {107, 20},
        {158, 35},
        {145, 142},
        {146, 210},
        {153, 274},
        {165, 321},
        {153, 328},
        {109, 321}
    };

    std::vector<pattern::VariantConstraint> constraints7 = {
        { .rotationAngle = 338, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 338, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(7, shape7, constraints7, sa, ca, g, true);

    std::vector<geometry::Point64> shape8 = {
        {370, 473},
        {384, 436},
        {386, 399},
        {380, 371},
        {373, 358},
        {361, 346},
        {362, 331},
        {410, 317},
        {442, 315},
        {533, 342},
        {533, 382},
        {525, 413},
        {496, 422},
        {480, 432},
        {455, 459},
        {433, 493},
        {370, 473}
    };

    std::vector<pattern::VariantConstraint> constraints8 = {
        { .rotationAngle = 136, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 136, .isMirrored = true, .requiredQuantity = 0, .twoWay = false }
    };

    patternPieces.emplace_back(8, shape8, constraints8, sa, ca, g, true);

    std::vector<geometry::Point64> shape9 = {
        {235, 479},
        {173, 502},
        {165, 492},
        {153, 485},
        {137, 480},
        {117, 476},
        {57, 475},
        {45, 389},
        {44, 325},
        {151, 331},
        {198, 344},
        {229, 357},
        {220, 386},
        {220, 440},
        {235, 479}
    };

    std::vector<pattern::VariantConstraint> constraints9 = {
        { .rotationAngle = 82, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 82, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(9, shape9, constraints9, sa, ca, g, true);

    std::vector<geometry::Point64> shape10 = {
        {499, 329},
        {445, 308},
        {457, 254},
        {463, 168},
        {463, 47},
        {465, 40},
        {504, 33},
        {577, 27},
        {576, 52},
        {559, 225},
        {555, 341},
        {499, 329}
    };

    std::vector<pattern::VariantConstraint> constraints10 = {
        { .rotationAngle = 63, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 63, .isMirrored = true, .requiredQuantity = 0, .twoWay = false }
    };

    patternPieces.emplace_back(10, shape10, constraints10, sa, ca, g, true);

    std::vector<geometry::Point64> shape11 = {
        {165, 321},
        {180, 262},
        {188, 194},
        {187, 123},
        {173, 36},
        {247, 31},
        {298, 35},
        {293, 140},
        {292, 212},
        {305, 305},
        {276, 314},
        {256, 327},
        {238, 350},
        {165, 321}
    };

    std::vector<pattern::VariantConstraint> constraints11 = {
        { .rotationAngle = 303, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 303, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(11, shape11, constraints11, sa, ca, g, true);

    std::vector<geometry::Point64> shape12 = {
        {358, 325},
        {335, 312},
        {314, 304},
        {329, 214},
        {330, 132},
        {327, 35},
        {384, 28},
        {425, 29},
        {463, 35},
        {445, 141},
        {435, 234},
        {439, 302},
        {434, 309},
        {390, 315},
        {358, 325}
    };

    std::vector<pattern::VariantConstraint> constraints12 = {
        { .rotationAngle = 339, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 339, .isMirrored = true, .requiredQuantity = 0, .twoWay = false }
    };

    patternPieces.emplace_back(12, shape12, constraints12, sa, ca, g, true);

    return patternPieces;
}

std::vector<pattern::PatternPiece> getTestPieces()
{
    int sa = 15;
    int ca = 5;
    int g = 0;

    std::vector<geometry::Point64> shape34 = {
        {1130, 655},
        {1157, 537},
        {1218, 434},
        {1255, 290},
        {1252, 196},
        {1260, 39},
        {1405, 51},
        {1351, 650},
        {1299, 645},
        {1258, 646},
        {1222, 659},
        {1200, 686},
        {1130, 655}
    };

    std::vector<pattern::VariantConstraint> constraints34 = {
        { .rotationAngle = 5, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = 5, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
    };

    std::vector<geometry::Point64> shape35 = {
        {1221, 1334},
        {1193, 1319},
        {1138, 1262},
        {1129, 1169},
        {1133, 1050},
        {1148, 923},
        {1183, 741},
        {1338, 751},
        {1334, 966},
        {1358, 1181},
        {1338, 1196},
        {1326, 1250},
        {1297, 1306},
        {1263, 1334},
        {1221, 1334}
    };

    std::vector<pattern::VariantConstraint> constraints35 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
    };

    std::vector<geometry::Point64> shape36 = {
        {872, 656},
        {885, 597},
        {883, 538},
        {908, 408},
        {920, 297},
        {915, 50},
        {1126, 58},
        {1120, 495},
        {1107, 614},
        {1089, 668},
        {1061, 633},
        {989, 652},
        {973, 698},
        {872, 656}
    };

    std::vector<pattern::VariantConstraint> constraints36 = {
        { .rotationAngle = 1, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = 1, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
    };

    std::vector<geometry::Point64> shape37 = {
        {293, 749},
        {255, 752},
        {244, 711},
        {176, 681},
        {135, 719},
        {127, 605},
        {130, 541},
        {175, 135},
        {297, 144},
        {269, 435},
        {269, 563},
        {293, 749}
    };

    std::vector<pattern::VariantConstraint> constraints37 = {
        { .rotationAngle = 6, .isMirrored = true, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = 6, .isMirrored = false, .requiredQuantity = 1, .twoWay = true }
    };

    std::vector<geometry::Point64> shape38 = {
        {122, 1411},
        {108, 1375},
        {69, 1342},
        {32, 1336},
        {55, 1147},
        {57, 1017},
        {54, 906},
        {143, 894},
        {158, 1079},
        {161, 1185},
        {150, 1314},
        {122, 1411}
    };

    std::vector<pattern::VariantConstraint> constraints38 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape39 = {
        {454, 1059},
        {305, 1026},
        {328, 882},
        {338, 872},
        {472, 889},
        {478, 903},
        {454, 1059}
    };

    std::vector<pattern::VariantConstraint> constraints39 = {
        { .rotationAngle = 8, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 8, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape40 = {
        {1409, 1226},
        {1417, 610},
        {1443, 50},
        {1573, 57},
        {1561, 614},
        {1562, 966},
        {1527, 1000},
        {1517, 1065},
        {1549, 1068},
        {1538, 1227},
        {1409, 1226}
    };

    std::vector<pattern::VariantConstraint> constraints40 = {
        { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape41 = {
        {223, 1501},
        {232, 1349},
        {779, 1377},
        {1373, 1427},
        {1372, 1479},
        {1210, 1486},
        {1150, 1496},
        {1106, 1539},
        {1093, 1598},
        {914, 1551},
        {223, 1501}
    };

    std::vector<pattern::VariantConstraint> constraints41 = {
        { .rotationAngle = 270+4, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 270+4, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape42 = {
        {495, 712},
        {447, 712},
        {425, 725},
        {302, 695},
        {324, 583},
        {367, 500},
        {407, 354},
        {407, 74},
        {528, 85},
        {505, 377},
        {507, 560},
        {495, 712}
    };

    std::vector<pattern::VariantConstraint> constraints42 = {
        { .rotationAngle = 3, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 3, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape43 = {
        {897, 1203},
        {880, 1163},
        {848, 1147},
        {868, 984},
        {863, 832},
        {870, 746},
        {970, 756},
        {954, 898},
        {937, 981},
        {936, 1092},
        {923, 1152},
        {897, 1203}
    };

    std::vector<pattern::VariantConstraint> constraints43 = {
        { .rotationAngle = 6, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 6, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape44 = {
        {254, 1119},
        {254, 1086},
        {304, 1062},
        {369, 1088},
        {437, 1090},
        {443, 1143},
        {362, 1143},
        {261, 1129},
        {254, 1119}
    };

    std::vector<pattern::VariantConstraint> constraints44 = {
        { .rotationAngle = 360-5, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 360-5, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape45 = {
        {547, 1171},
        {477, 1159},
        {476, 1008},
        {517, 526},
        {536, 53},
        {682, 58},
        {652, 1042},
        {631, 1187},
        {547, 1171}
    };

    std::vector<pattern::VariantConstraint> constraints45 = {
        { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape46 = {
        {811, 1199},
        {674, 1193},
        {677, 913},
        {723, 56},
        {847, 61},
        {822, 746},
        {824, 1093},
        {811, 1199}
    };

    std::vector<pattern::VariantConstraint> constraints46 = {
        { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape47 = {
        {1036, 1366},
        {966, 1337},
        {979, 1277},
        {979, 1224},
        {1007, 1059},
        {1006, 748},
        {1097, 749},
        {1090, 1133},
        {1063, 1280},
        {1036, 1366}
    };

    std::vector<pattern::VariantConstraint> constraints47 = {
        { .rotationAngle = 1, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 1, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape48 = {
        {276, 811},
        {286, 776},
        {475, 785},
        {454, 834},
        {381, 830},
        {323, 846},
        {276, 811}
    };

    std::vector<pattern::VariantConstraint> constraints48 = {
        { .rotationAngle = 180+17, .isMirrored = true, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 180+17, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    std::vector<geometry::Point64> shape49 = {
        {445, 1231},
        {430, 1211},
        {402, 1199},
        {369, 1193},
        {370, 1154},
        {402, 1158},
        {449, 1175},
        {481, 1219},
        {445, 1231}
    };

    std::vector<pattern::VariantConstraint> constraints49 = {
        { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = true }
    };

    std::vector<geometry::Point64> shape50 = {
        {898, 1380},
        {650, 1341},
        {536, 1339},
        {462, 1327},
        {402, 1298},
        {467, 1280},
        {491, 1242},
        {658, 1248},
        {780, 1243},
        {902, 1248},
        {898, 1380}
    };

    std::vector<pattern::VariantConstraint> constraints50 = {
        { .rotationAngle = 90+4, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = 90+4, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
    };

    std::vector<geometry::Point64> shape51 = {
        {77, 826},
        {67, 789},
        {133, 770},
        {213, 770},
        {263, 775},
        {253, 815},
        {198, 810},
        {135, 810},
        {77, 826}
    };

    std::vector<pattern::VariantConstraint> constraints51 = {
        { .rotationAngle = 6, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
        { .rotationAngle = 6, .isMirrored = true, .requiredQuantity = 2, .twoWay = true }
    };

    std::vector<geometry::Point64> shape52 = {
        {206, 1180},
        {206, 1138},
        {353, 1143},
        {353, 1185},
        {206, 1180}
    };

    std::vector<pattern::VariantConstraint> constraints52 = {
        { .rotationAngle = 1, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
        { .rotationAngle = 1, .isMirrored = true, .requiredQuantity = 0, .twoWay = true }
    };

    std::vector<geometry::Point64> shape53 = {
        {246, 839},
        {237, 858},
        {151, 865},
        {106, 859},
        {107, 840},
        {152, 845},
        {246, 839}
    };

    std::vector<pattern::VariantConstraint> constraints53 = {
        { .rotationAngle = 180, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
        { .rotationAngle = 180, .isMirrored = true, .requiredQuantity = 2, .twoWay = true }
    };

    std::vector<geometry::Point64> shape54 = {
        {284, 1039},
        {297, 875},
        {246, 871},
        {232, 1034},
        {284, 1039}
    };

    std::vector<pattern::VariantConstraint> constraints54 = {
        { .rotationAngle = 5, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = 5, .isMirrored = true, .requiredQuantity = 0, .twoWay = true }
    };

    std::vector<geometry::Point64> shape55 = {
        {218, 1081},
        {181, 1095},
        {168, 1017},
        {179, 889},
        {219, 896},
        {211, 1010},
        {218, 1081}
    };

    std::vector<pattern::VariantConstraint> constraints55 = {
        { .rotationAngle = 270+3, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
        { .rotationAngle = 270+3, .isMirrored = true, .requiredQuantity = 2, .twoWay = true }
    };

    // allPatternPieces.emplace_back(12436, shape34, constraints34, sa, ca, g, true);
    // allPatternPieces.emplace_back(12435, shape47, constraints47, sa, ca, g, true);

    std::vector<pattern::PatternPiece> suit;
    suit.reserve(19);

    suit.emplace_back(12428, shape35, constraints35, sa, ca, g, true);
    suit.emplace_back(12421, shape36, constraints36, sa, ca, g, true);
    suit.emplace_back(12430, shape37, constraints37, sa, ca, g, true);
    suit.emplace_back(12429, shape38, constraints38, sa, ca, g, true);
    suit.emplace_back(12434, shape39, constraints39, sa, ca, g, true);
    suit.emplace_back(12423, shape42, constraints42, sa, ca, g, true);
    suit.emplace_back(12422, shape43, constraints43, sa, ca, g, true);
    suit.emplace_back(12425, shape44, constraints44, sa, ca, g, true);
    suit.emplace_back(12426, shape48, constraints48, sa, ca, g, true);
    suit.emplace_back(12431, shape49, constraints49, sa, ca, g, true);
    suit.emplace_back(12424, shape50, constraints50, sa, ca, g, true);
    suit.emplace_back(12433, shape52, constraints52, sa, ca, g, true);
    suit.emplace_back(12427, shape53, constraints53, sa, ca, g, true);


    suit.emplace_back(12521, shape40, constraints40, sa, ca, g, true);
    suit.emplace_back(12523, shape41, constraints41, sa, ca, g, true);
    suit.emplace_back(12524, shape45, constraints45, sa, ca, g, true);
    suit.emplace_back(12522, shape46, constraints46, sa, ca, g, true);
    suit.emplace_back(12526, shape51, constraints51, sa, ca, g, true);
    suit.emplace_back(12527, shape54, constraints54, sa, ca, g, true);
    suit.emplace_back(12525, shape55, constraints55, sa, ca, g, true);

    return suit;
}

} // namespace pmg::samples