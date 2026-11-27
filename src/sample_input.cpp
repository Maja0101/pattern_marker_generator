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

    patternPieces.emplace_back(1, shape1, constraints1, sa, ca, true);

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

    patternPieces.emplace_back(2, shape2, constraints2, sa, ca, true);

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

    patternPieces.emplace_back(3, shape3, constraints3, sa, ca, true);

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

    patternPieces.emplace_back(4, shape4, constraints4, sa, ca, true);

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

    patternPieces.emplace_back(5, shape5, constraints5, sa, ca, true);

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

    patternPieces.emplace_back(6, shape6, constraints6, sa, ca, true);

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

    patternPieces.emplace_back(7, shape7, constraints7, sa, ca, true);

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

    patternPieces.emplace_back(8, shape8, constraints8, sa, ca, true);

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

    patternPieces.emplace_back(7, shape7, constraints7, sa, ca, true);

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

    patternPieces.emplace_back(8, shape8, constraints8, sa, ca, true);

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

    patternPieces.emplace_back(9, shape9, constraints9, sa, ca, true);

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

    patternPieces.emplace_back(10, shape10, constraints10, sa, ca, true);

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

    patternPieces.emplace_back(11, shape11, constraints11, sa, ca, true);

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

    patternPieces.emplace_back(12, shape12, constraints12, sa, ca, true);

    return patternPieces;
}

std::vector<pattern::PatternPiece> getPiecesToCheck3()
{
    std::vector<pattern::PatternPiece> patternPieces;
    patternPieces.reserve(6);

    std::vector<geometry::Point64> shape1 = {
        {624200, 257100},
        {386900, 241800},
        {272600, 244300},
        {271800, 117400},
        {343300, 114100},
        {411800, 96800},
        {561000, 27500}
    };

    std::vector<pattern::VariantConstraint> constraints1 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(1, shape1, constraints1, 5, 20, false);

    std::vector<geometry::Point64> shape2 = {
        {202000, 955700},
        {190400, 942300},
        {162300, 934100},
        {92900, 932500},
        {94400, 795000},
        {117500, 379400},
        {255000, 380200},
        {267000, 831000},
        {285500, 927800}
    };

    std::vector<pattern::VariantConstraint> constraints2 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(2, shape2, constraints2, 5, 20, false);

    std::vector<geometry::Point64> shape3 = {
        {484800, 855200},
        {523100, 781800},
        {539500, 734400},
        {542500, 691900},
        {535800, 671800},
        {521900, 653600},
        {498800, 643900},
        {480000, 640300},
        {499400, 368200},
        {647000, 363400},
        {664600, 575900},
        {578900, 913500}
    };

    std::vector<pattern::VariantConstraint> constraints3 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(3, shape3, constraints3, 5, 20, false);

    std::vector<geometry::Point64> shape4 = {
        {335900, 926700},
        {312800, 835100},
        {302800, 703700},
        {315900, 370100},
        {451400, 365000},
        {465400, 638500},
        {437600, 645300},
        {421400, 659700},
        {410000, 675400},
        {404400, 703700},
        {406400, 774200},
        {419200, 834500},
        {438000, 870400},
        {384600, 904600}
    };

    std::vector<pattern::VariantConstraint> constraints4 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(4, shape4, constraints4, 5, 20, false);

    std::vector<geometry::Point64> shape5 = {
        {764700, 967600},
        {767600, 361800},
        {901800, 358800},
        {911200, 839900},
        {868400, 842300},
        {847700, 845800},
        {831800, 857900},
        {810800, 878200},
        {796800, 906900},
        {776900, 968200}
    };

    std::vector<pattern::VariantConstraint> constraints5 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = 0, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(5, shape5, constraints5, 5, 20, false);

    std::vector<geometry::Point64> shape6 = {
        {748700, 938800},
        {664600, 908700},
        {667900, 694600},
        {678000, 358600},
        {738800, 359100},
        {748200, 616400}
    };

    std::vector<pattern::VariantConstraint> constraints6 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(6, shape6, constraints6, 5, 20, false);

    return patternPieces;
}

std::vector<pattern::PatternPiece> getPiecesToCheck4()
{
    std::vector<pattern::PatternPiece> patternPieces;
    patternPieces.reserve(9);

    std::vector<geometry::Point64> shape1 = {
        {532640, 540640},
        {546720, 485600},
        {546720, 457600},
        {766960, 464560},
        {761920, 486640},
        {770960, 545680},
        {721920, 580720},
        {689840, 596720},
        {662800, 603760},
        {636800, 603760},
        {618800, 598720},
        {557680, 550720}
    };

    std::vector<pattern::VariantConstraint> constraints1 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(1, shape1, constraints1, 5, 20, false);

    std::vector<geometry::Point64> shape2 = {
        {504640, 725920},
        {504640, 666800},
        {508640, 610800},
        {519680, 599760},
        {599760, 600720},
        {606720, 609760},
        {602720, 724880}
    };

    std::vector<pattern::VariantConstraint> constraints2 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(2, shape2, constraints2, 5, 20, false);

    std::vector<geometry::Point64> shape3 = {
        {648800, 441520},
        {553680, 383440},
        {585760, 242320},
        {592720, 83120},
        {772000, 85120},
        {770000, 254320},
        {790960, 384480},
        {718880, 435520},
        {681840, 445520}
    };

    std::vector<pattern::VariantConstraint> constraints3 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(3, shape3, constraints3, 5, 20, false);

    std::vector<geometry::Point64> shape4 = {
        {626800, 724880},
        {627760, 654800},
        {631760, 628800},
        {645840, 617760},
        {700880, 617760},
        {709920, 629760},
        {707920, 702880},
        {704880, 723920}
    };

    std::vector<pattern::VariantConstraint> constraints4 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(4, shape4, constraints4, 5, 20, false);

    std::vector<geometry::Point64> shape5 = {
        {357440, 582720},
        {357440, 106160},
        {565680, 106160},
        {564720, 172240},
        {536640, 364480},
        {536640, 453600},
        {515680, 466560},
        {500640, 489600},
        {506640, 576720},
        {435520, 593760},
        {409520, 581760}
    };

    std::vector<pattern::VariantConstraint> constraints5 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(5, shape5, constraints5, 5, 20, false);

    std::vector<geometry::Point64> shape6 = {
        {193280, 603760},
        {174240, 565680},
        {128160, 550720},
        {135200, 467600},
        {135200, 88080},
        {337440, 95120},
        {337440, 153200},
        {307360, 349440},
        {305360, 458560},
        {277360, 471600},
        {261360, 498640},
        {271360, 589760}
    };

    std::vector<pattern::VariantConstraint> constraints6 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(6, shape6, constraints6, 5, 20, false);

    std::vector<geometry::Point64> shape7 = {
        {66080, 694880},
        {67120, 672880},
        {75120, 649840},
        {143200, 639760},
        {219280, 635760},
        {307360, 636800},
        {323440, 642800},
        {328400, 683840}
    };

    std::vector<pattern::VariantConstraint> constraints7 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(7, shape7, constraints7, 5, 20, false);

    std::vector<geometry::Point64> shape8 = {
        {78080, 611760},
        {55040, 571680},
        {7040, 555680},
        {21040, 446560},
        {35040, 89120},
        {91120, 89120},
        {77120, 361440},
        {89120, 460560},
        {123120, 604720}
    };

    std::vector<pattern::VariantConstraint> constraints8 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(8, shape8, constraints8, 5, 20, false);

    std::vector<geometry::Point64> shape9 = {
        {346400, 661840},
        {346400, 651840},
        {351440, 643840},
        {357440, 638800},
        {469600, 633760},
        {473600, 636800},
        {477600, 644800},
        {477600, 657840},
        {470560, 669840},
        {465600, 671840},
        {417520, 671840},
        {386480, 674880},
        {354480, 674880}
    };

    std::vector<pattern::VariantConstraint> constraints9 = {
        { .rotationAngle = 0, .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
    };

    patternPieces.emplace_back(9, shape9, constraints9, 5, 20, false);

    return patternPieces;
}

std::vector<pattern::PatternPiece> getTestPieces()
{
    int sa = 15;
    int ca = 5;

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
        { .rotationAngle = -5, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = -5, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
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
        { .rotationAngle = -1, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = -1, .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
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
        { .rotationAngle = -6, .isMirrored = true, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = -6, .isMirrored = false, .requiredQuantity = 1, .twoWay = true }
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
        { .rotationAngle = -8, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -8, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -(270+4), .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -(270+4), .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -3, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -3, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -6, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -6, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -(360-5), .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -(360-5), .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -2, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -2, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -1, .isMirrored = false, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -1, .isMirrored = true, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -(180+17), .isMirrored = true, .requiredQuantity = 1, .twoWay = false },
        { .rotationAngle = -(180+17), .isMirrored = false, .requiredQuantity = 1, .twoWay = false }
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
        { .rotationAngle = -(90+4), .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = -(90+4), .isMirrored = true, .requiredQuantity = 1, .twoWay = true }
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
        { .rotationAngle = -6, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
        { .rotationAngle = -6, .isMirrored = true, .requiredQuantity = 2, .twoWay = true }
    };

    std::vector<geometry::Point64> shape52 = {
        {206, 1180},
        {206, 1138},
        {353, 1143},
        {353, 1185},
        {206, 1180}
    };

    std::vector<pattern::VariantConstraint> constraints52 = {
        { .rotationAngle = -1, .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
        { .rotationAngle = -1, .isMirrored = true, .requiredQuantity = 0, .twoWay = true }
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
        { .rotationAngle = -5, .isMirrored = false, .requiredQuantity = 1, .twoWay = true },
        { .rotationAngle = -5, .isMirrored = true, .requiredQuantity = 0, .twoWay = true }
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
        { .rotationAngle = -(270+3), .isMirrored = false, .requiredQuantity = 2, .twoWay = true },
        { .rotationAngle = -(270+3), .isMirrored = true, .requiredQuantity = 2, .twoWay = true }
    };

    // allPatternPieces.emplace_back(12436, shape34, constraints34, sa, ca, true);
    // allPatternPieces.emplace_back(12435, shape47, constraints47, sa, ca, true);

    std::vector<pattern::PatternPiece> suit;
    suit.reserve(19);

    suit.emplace_back(12428, shape35, constraints35, sa, ca, true);
    suit.emplace_back(12421, shape36, constraints36, sa, ca, true);
    suit.emplace_back(12430, shape37, constraints37, sa, ca, true);
    suit.emplace_back(12429, shape38, constraints38, sa, ca, true);
    suit.emplace_back(12434, shape39, constraints39, sa, ca, true);
    suit.emplace_back(12423, shape42, constraints42, sa, ca, true);
    suit.emplace_back(12422, shape43, constraints43, sa, ca, true);
    suit.emplace_back(12425, shape44, constraints44, sa, ca, true);
    suit.emplace_back(12426, shape48, constraints48, sa, ca, true);
    suit.emplace_back(12431, shape49, constraints49, sa, ca, true);
    suit.emplace_back(12424, shape50, constraints50, sa, ca, true);
    suit.emplace_back(12433, shape52, constraints52, sa, ca, true);
    suit.emplace_back(12427, shape53, constraints53, sa, ca, true);


    suit.emplace_back(12521, shape40, constraints40, sa, ca, true);
    suit.emplace_back(12523, shape41, constraints41, sa, ca, true);
    suit.emplace_back(12524, shape45, constraints45, sa, ca, true);
    suit.emplace_back(12522, shape46, constraints46, sa, ca, true);
    suit.emplace_back(12526, shape51, constraints51, sa, ca, true);
    suit.emplace_back(12527, shape54, constraints54, sa, ca, true);
    suit.emplace_back(12525, shape55, constraints55, sa, ca, true);

    return suit;
}

} // namespace pmg::samples