#include "utils_benchmark/benchmark_conf.hpp"

namespace pmg::bench {

std::vector<int64_t> getBenchIds() {
    return {
        12428, 12421, 12430, 12429, 12434, 12423, 12422, 12425, 12426, 12431, 12424, 12433, 12427,
        12521, 12523, 12524, 12522, 12526, 12527, 12525,
        12436, 12435
    };
};

std::vector<std::vector<geometry::Point64>> getBenchShapes() {
    return {
        {
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
        },
        {
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
        },
        {
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
        },
        {
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
        },
        {
            {454, 1059},
            {305, 1026},
            {328, 882},
            {338, 872},
            {472, 889},
            {478, 903},
            {454, 1059}
        },
        {
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
        },
        {
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
        },
        {
            {254, 1119},
            {254, 1086},
            {304, 1062},
            {369, 1088},
            {437, 1090},
            {443, 1143},
            {362, 1143},
            {261, 1129},
            {254, 1119}
        },
        {
            {276, 811},
            {286, 776},
            {475, 785},
            {454, 834},
            {381, 830},
            {323, 846},
            {276, 811}
        },
        {
            {445, 1231},
            {430, 1211},
            {402, 1199},
            {369, 1193},
            {370, 1154},
            {402, 1158},
            {449, 1175},
            {481, 1219},
            {445, 1231}
        },
        {
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
        },
        {
            {206, 1180},
            {206, 1138},
            {353, 1143},
            {353, 1185},
            {206, 1180}
        },
        {
            {246, 839},
            {237, 858},
            {151, 865},
            {106, 859},
            {107, 840},
            {152, 845},
            {246, 839}
        },
        {
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
        },
        {
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
        },
        {
            {547, 1171},
            {477, 1159},
            {476, 1008},
            {517, 526},
            {536, 53},
            {682, 58},
            {652, 1042},
            {631, 1187},
            {547, 1171}
        },
        {
            {811, 1199},
            {674, 1193},
            {677, 913},
            {723, 56},
            {847, 61},
            {822, 746},
            {824, 1093},
            {811, 1199}
        },
        {
            {77, 826},
            {67, 789},
            {133, 770},
            {213, 770},
            {263, 775},
            {253, 815},
            {198, 810},
            {135, 810},
            {77, 826}
        },
        {
            {284, 1039},
            {297, 875},
            {246, 871},
            {232, 1034},
            {284, 1039}
        },
        {
            {218, 1081},
            {181, 1095},
            {168, 1017},
            {179, 889},
            {219, 896},
            {211, 1010},
            {218, 1081}
        },
        {
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
        },
        {
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
        }
    };
};

std::vector<std::vector<pattern::VariantConstraint>> getBenchConstraints() {
    return {
        {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = -1, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = -1, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = -6, .isMirrored = true, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = -6, .isMirrored = false, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = -8, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = -8, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = -3, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = -3, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = -6, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = -6, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = -(360-5), .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = -(360-5), .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = -(180+17), .isMirrored = true, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = -(180+17), .isMirrored = false, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 90+4, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 90+4, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 1, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
            { .rotationAngle = 1, .isMirrored = true, .requiredQuantity = 0, .twoWay = true }
        },
        {
            { .rotationAngle = 180, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
            { .rotationAngle = 180, .isMirrored = true, .requiredQuantity = 2, .twoWay = true }
        },
        {
            { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 270+4, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 270+4, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 2, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 2, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 6, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
            { .rotationAngle = 6, .isMirrored = true, .requiredQuantity = 2, .twoWay = true }
        },
        {
            { .rotationAngle = 5, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 5, .isMirrored = true, .requiredQuantity = 0, .twoWay = true }
        },
        {
            { .rotationAngle = 270+3, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
            { .rotationAngle = 270+3, .isMirrored = true, .requiredQuantity = 2, .twoWay = true }
        },
        {
            { .rotationAngle = 5, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 5, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        },
        {
            { .rotationAngle = 1, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
            { .rotationAngle = 1, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
        }
    };
};

} // namespace pmg::bench