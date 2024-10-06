#ifndef WAVEFORMS_H
#define WAVEFORMS_H

// Define LUT size
const int LUT_SIZE = 512; 

// Ramp-up LUT
const uint16_t rampUpLUT[LUT_SIZE] = 
{
  0, 6, 12, 18, 24, 30, 36, 42, 48, 54,
  60, 66, 72, 78, 84, 90, 96, 102, 108, 114,
  120, 126, 133, 139, 145, 151, 157, 163, 169, 175,
  181, 187, 193, 199, 205, 211, 217, 223, 229, 235,
  241, 247, 253, 260, 266, 272, 278, 284, 290, 296,
  302, 308, 314, 320, 326, 332, 338, 344, 350, 356,
  362, 368, 374, 380, 387, 393, 399, 405, 411, 417,
  423, 429, 435, 441, 447, 453, 459, 465, 471, 477,
  483, 489, 495, 501, 507, 513, 520, 526, 532, 538,
  544, 550, 556, 562, 568, 574, 580, 586, 592, 598,
  604, 610, 616, 622, 628, 634, 640, 647, 653, 659,
  665, 671, 677, 683, 689, 695, 701, 707, 713, 719,
  725, 731, 737, 743, 749, 755, 761, 767, 774, 780,
  786, 792, 798, 804, 810, 816, 822, 828, 834, 840,
  846, 852, 858, 864, 870, 876, 882, 888, 894, 900,
  907, 913, 919, 925, 931, 937, 943, 949, 955, 961,
  967, 973, 979, 985, 991, 997, 1003, 1009, 1015, 1021,
  1027, 1034, 1040, 1046, 1052, 1058, 1064, 1070, 1076, 1082,
  1088, 1094, 1100, 1106, 1112, 1118, 1124, 1130, 1136, 1142,
  1148, 1154, 1161, 1167, 1173, 1179, 1185, 1191, 1197, 1203,
  1209, 1215, 1221, 1227, 1233, 1239, 1245, 1251, 1257, 1263,
  1269, 1275, 1281, 1288, 1294, 1300, 1306, 1312, 1318, 1324,
  1330, 1336, 1342, 1348, 1354, 1360, 1366, 1372, 1378, 1384,
  1390, 1396, 1402, 1408, 1414, 1421, 1427, 1433, 1439, 1445,
  1451, 1457, 1463, 1469, 1475, 1481, 1487, 1493, 1499, 1505,
  1511, 1517, 1523, 1529, 1535, 1541, 1548, 1554, 1560, 1566,
  1572, 1578, 1584, 1590, 1596, 1602, 1608, 1614, 1620, 1626,
  1632, 1638, 1644, 1650, 1656, 1662, 1668, 1675, 1681, 1687,
  1693, 1699, 1705, 1711, 1717, 1723, 1729, 1735, 1741, 1747,
  1753, 1759, 1765, 1771, 1777, 1783, 1789, 1795, 1801, 1808,
  1814, 1820, 1826, 1832, 1838, 1844, 1850, 1856, 1862, 1868,
  1874, 1880, 1886, 1892, 1898, 1904, 1910, 1916, 1922, 1928,
  1935, 1941, 1947, 1953, 1959, 1965, 1971, 1977, 1983, 1989,
  1995, 2001, 2007, 2013, 2019, 2025, 2031, 2037, 2043, 2049,
  2055, 2062, 2068, 2074, 2080, 2086, 2092, 2098, 2104, 2110,
  2116, 2122, 2128, 2134, 2140, 2146, 2152, 2158, 2164, 2170,
  2176, 2182, 2189, 2195, 2201, 2207, 2213, 2219, 2225, 2231,
  2237, 2243, 2249, 2255, 2261, 2267, 2273, 2279, 2285, 2291,
  2297, 2303, 2309, 2315, 2322, 2328, 2334, 2340, 2346, 2352,
  2358, 2364, 2370, 2376, 2382, 2388, 2394, 2400, 2406, 2412,
  2418, 2424, 2430, 2436, 2442, 2449, 2455, 2461, 2467, 2473,
  2479, 2485, 2491, 2497, 2503, 2509, 2515, 2521, 2527, 2533,
  2539, 2545, 2551, 2557, 2563, 2569, 2576, 2582, 2588, 2594,
  2600, 2606, 2612, 2618, 2624, 2630, 2636, 2642, 2648, 2654,
  2660, 2666, 2672, 2678, 2684, 2690, 2696, 2702, 2709, 2715,
  2721, 2727, 2733, 2739, 2745, 2751, 2757, 2763, 2769, 2775,
  2781, 2787, 2793, 2799, 2805, 2811, 2817, 2823, 2829, 2836,
  2842, 2848, 2854, 2860, 2866, 2872, 2878, 2884, 2890, 2896,
  2902, 2908, 2914, 2920, 2926, 2932, 2938, 2944, 2950, 2956,
  2963, 2969, 2975, 2981, 2987, 2993, 2999, 3005, 3011, 3017,
  3023, 3029, 3035, 3041, 3047, 3053, 3059, 3065, 3071, 3077,
  3083, 3090
};



// Ramp-down LUT
const uint16_t rampDownLUT[LUT_SIZE] = 
{
  3090, 3083, 3077, 3071, 3065, 3059, 3053, 3047, 3041, 3035,
  3029, 3023, 3017, 3011, 3005, 2999, 2993, 2987, 2981, 2975,
  2969, 2963, 2956, 2950, 2944, 2938, 2932, 2926, 2920, 2914,
  2908, 2902, 2896, 2890, 2884, 2878, 2872, 2866, 2860, 2854,
  2848, 2842, 2836, 2829, 2823, 2817, 2811, 2805, 2799, 2793,
  2787, 2781, 2775, 2769, 2763, 2757, 2751, 2745, 2739, 2733,
  2727, 2721, 2715, 2709, 2702, 2696, 2690, 2684, 2678, 2672,
  2666, 2660, 2654, 2648, 2642, 2636, 2630, 2624, 2618, 2612,
  2606, 2600, 2594, 2588, 2582, 2576, 2569, 2563, 2557, 2551,
  2545, 2539, 2533, 2527, 2521, 2515, 2509, 2503, 2497, 2491,
  2485, 2479, 2473, 2467, 2461, 2455, 2449, 2442, 2436, 2430,
  2424, 2418, 2412, 2406, 2400, 2394, 2388, 2382, 2376, 2370,
  2364, 2358, 2352, 2346, 2340, 2334, 2328, 2322, 2315, 2309,
  2303, 2297, 2291, 2285, 2279, 2273, 2267, 2261, 2255, 2249,
  2243, 2237, 2231, 2225, 2219, 2213, 2207, 2201, 2195, 2189,
  2182, 2176, 2170, 2164, 2158, 2152, 2146, 2140, 2134, 2128,
  2122, 2116, 2110, 2104, 2098, 2092, 2086, 2080, 2074, 2068,
  2062, 2055, 2049, 2043, 2037, 2031, 2025, 2019, 2013, 2007,
  2001, 1995, 1989, 1983, 1977, 1971, 1965, 1959, 1953, 1947,
  1941, 1935, 1928, 1922, 1916, 1910, 1904, 1898, 1892, 1886,
  1880, 1874, 1868, 1862, 1856, 1850, 1844, 1838, 1832, 1826,
  1820, 1814, 1808, 1801, 1795, 1789, 1783, 1777, 1771, 1765,
  1759, 1753, 1747, 1741, 1735, 1729, 1723, 1717, 1711, 1705,
  1699, 1693, 1687, 1681, 1675, 1668, 1662, 1656, 1650, 1644,
  1638, 1632, 1626, 1620, 1614, 1608, 1602, 1596, 1590, 1584,
  1578, 1572, 1566, 1560, 1554, 1548, 1541, 1535, 1529, 1523,
  1517, 1511, 1505, 1499, 1493, 1487, 1481, 1475, 1469, 1463,
  1457, 1451, 1445, 1439, 1433, 1427, 1421, 1414, 1408, 1402,
  1396, 1390, 1384, 1378, 1372, 1366, 1360, 1354, 1348, 1342,
  1336, 1330, 1324, 1318, 1312, 1306, 1300, 1294, 1288, 1281,
  1275, 1269, 1263, 1257, 1251, 1245, 1239, 1233, 1227, 1221,
  1215, 1209, 1203, 1197, 1191, 1185, 1179, 1173, 1167, 1161,
  1154, 1148, 1142, 1136, 1130, 1124, 1118, 1112, 1106, 1100,
  1094, 1088, 1082, 1076, 1070, 1064, 1058, 1052, 1046, 1040,
  1034, 1027, 1021, 1015, 1009, 1003, 997, 991, 985, 979,
  973, 967, 961, 955, 949, 943, 937, 931, 925, 919,
  913, 907, 900, 894, 888, 882, 876, 870, 864, 858,
  852, 846, 840, 834, 828, 822, 816, 810, 804, 798,
  792, 786, 780, 774, 767, 761, 755, 749, 743, 737,
  731, 725, 719, 713, 707, 701, 695, 689, 683, 677,
  671, 665, 659, 653, 647, 640, 634, 628, 622, 616,
  610, 604, 598, 592, 586, 580, 574, 568, 562, 556,
  550, 544, 538, 532, 526, 520, 513, 507, 501, 495,
  489, 483, 477, 471, 465, 459, 453, 447, 441, 435,
  429, 423, 417, 411, 405, 399, 393, 387, 380, 374,
  368, 362, 356, 350, 344, 338, 332, 326, 320, 314,
  308, 302, 296, 290, 284, 278, 272, 266, 260, 253,
  247, 241, 235, 229, 223, 217, 211, 205, 199, 193,
  187, 181, 175, 169, 163, 157, 151, 145, 139, 133,
  126, 120, 114, 108, 102, 96, 90, 84, 78, 72,
  66, 60, 54, 48, 42, 36, 30, 24, 18, 12,
  6, 0
};


     
// sine LUT
const uint16_t sineLUT[LUT_SIZE] = 
{
  0, 0, 0, 1, 1, 2, 4, 5, 7, 9,
  11, 14, 16, 19, 22, 26, 29, 33, 37, 41,
  46, 51, 55, 61, 66, 72, 77, 84, 90, 96,
  103, 110, 117, 124, 132, 140, 148, 156, 164, 173,
  182, 191, 200, 210, 219, 229, 239, 249, 260, 271,
  281, 292, 304, 315, 327, 338, 350, 362, 375, 387,
  400, 413, 426, 439, 452, 466, 479, 493, 507, 521,
  535, 550, 564, 579, 594, 609, 624, 639, 655, 670,
  686, 702, 718, 734, 750, 767, 783, 800, 816, 833,
  850, 867, 884, 901, 918, 936, 953, 971, 988, 1006,
  1024, 1042, 1060, 1078, 1096, 1114, 1132, 1151, 1169, 1188,
  1206, 1225, 1243, 1262, 1280, 1299, 1318, 1337, 1355, 1374,
  1393, 1412, 1431, 1450, 1469, 1488, 1507, 1526, 1545, 1563,
  1582, 1601, 1620, 1639, 1658, 1677, 1696, 1715, 1734, 1752,
  1771, 1790, 1809, 1827, 1846, 1864, 1883, 1901, 1920, 1938,
  1957, 1975, 1993, 2011, 2029, 2047, 2065, 2083, 2101, 2118,
  2136, 2153, 2171, 2188, 2205, 2222, 2239, 2256, 2273, 2289,
  2306, 2322, 2339, 2355, 2371, 2387, 2403, 2419, 2434, 2450,
  2465, 2480, 2495, 2510, 2525, 2539, 2554, 2568, 2582, 2596,
  2610, 2623, 2637, 2650, 2663, 2676, 2689, 2702, 2714, 2727,
  2739, 2751, 2762, 2774, 2785, 2797, 2808, 2818, 2829, 2840,
  2850, 2860, 2870, 2879, 2889, 2898, 2907, 2916, 2925, 2933,
  2941, 2949, 2957, 2965, 2972, 2979, 2986, 2993, 2999, 3005,
  3012, 3017, 3023, 3028, 3034, 3038, 3043, 3048, 3052, 3056,
  3060, 3063, 3067, 3070, 3073, 3075, 3078, 3080, 3082, 3084,
  3085, 3087, 3088, 3088, 3089, 3089, 3090, 3089, 3089, 3088,
  3088, 3087, 3085, 3084, 3082, 3080, 3078, 3075, 3073, 3070,
  3067, 3063, 3060, 3056, 3052, 3048, 3043, 3038, 3034, 3028,
  3023, 3017, 3012, 3005, 2999, 2993, 2986, 2979, 2972, 2965,
  2957, 2949, 2941, 2933, 2925, 2916, 2907, 2898, 2889, 2879,
  2870, 2860, 2850, 2840, 2829, 2818, 2808, 2797, 2785, 2774,
  2762, 2751, 2739, 2727, 2714, 2702, 2689, 2676, 2663, 2650,
  2637, 2623, 2610, 2596, 2582, 2568, 2554, 2539, 2525, 2510,
  2495, 2480, 2465, 2450, 2434, 2419, 2403, 2387, 2371, 2355,
  2339, 2322, 2306, 2289, 2273, 2256, 2239, 2222, 2205, 2188,
  2171, 2153, 2136, 2118, 2101, 2083, 2065, 2047, 2029, 2011,
  1993, 1975, 1957, 1938, 1920, 1901, 1883, 1864, 1846, 1827,
  1809, 1790, 1771, 1752, 1734, 1715, 1696, 1677, 1658, 1639,
  1620, 1601, 1582, 1563, 1545, 1526, 1507, 1488, 1469, 1450,
  1431, 1412, 1393, 1374, 1355, 1337, 1318, 1299, 1280, 1262,
  1243, 1225, 1206, 1188, 1169, 1151, 1132, 1114, 1096, 1078,
  1060, 1042, 1024, 1006, 988, 971, 953, 936, 918, 901,
  884, 867, 850, 833, 816, 800, 783, 767, 750, 734,
  718, 702, 686, 670, 655, 639, 624, 609, 594, 579,
  564, 550, 535, 521, 507, 493, 479, 466, 452, 439,
  426, 413, 400, 387, 375, 362, 350, 338, 327, 315,
  304, 292, 281, 271, 260, 249, 239, 229, 219, 210,
  200, 191, 182, 173, 164, 156, 148, 140, 132, 124,
  117, 110, 103, 96, 90, 84, 77, 72, 66, 61,
  55, 51, 46, 41, 37, 33, 29, 26, 22, 19,
  16, 14, 11, 9, 7, 5, 4, 2, 1, 1,
  0, 0
};



// Triangle LUT
const uint16_t triangleLUT[LUT_SIZE] = 
{
  0, 12, 24, 36, 48, 60, 72, 84, 96, 108,
  120, 132, 144, 156, 168, 181, 193, 205, 217, 229,
  241, 253, 265, 277, 289, 301, 313, 325, 337, 350,
  362, 374, 386, 398, 410, 422, 434, 446, 458, 470,
  482, 494, 506, 519, 531, 543, 555, 567, 579, 591,
  603, 615, 627, 639, 651, 663, 675, 688, 700, 712,
  724, 736, 748, 760, 772, 784, 796, 808, 820, 832,
  844, 856, 869, 881, 893, 905, 917, 929, 941, 953,
  965, 977, 989, 1001, 1013, 1025, 1038, 1050, 1062, 1074,
  1086, 1098, 1110, 1122, 1134, 1146, 1158, 1170, 1182, 1194,
  1207, 1219, 1231, 1243, 1255, 1267, 1279, 1291, 1303, 1315,
  1327, 1339, 1351, 1363, 1376, 1388, 1400, 1412, 1424, 1436,
  1448, 1460, 1472, 1484, 1496, 1508, 1520, 1532, 1545, 1557,
  1569, 1581, 1593, 1605, 1617, 1629, 1641, 1653, 1665, 1677,
  1689, 1701, 1713, 1726, 1738, 1750, 1762, 1774, 1786, 1798,
  1810, 1822, 1834, 1846, 1858, 1870, 1882, 1895, 1907, 1919,
  1931, 1943, 1955, 1967, 1979, 1991, 2003, 2015, 2027, 2039,
  2051, 2064, 2076, 2088, 2100, 2112, 2124, 2136, 2148, 2160,
  2172, 2184, 2196, 2208, 2220, 2233, 2245, 2257, 2269, 2281,
  2293, 2305, 2317, 2329, 2341, 2353, 2365, 2377, 2389, 2401,
  2414, 2426, 2438, 2450, 2462, 2474, 2486, 2498, 2510, 2522,
  2534, 2546, 2558, 2570, 2583, 2595, 2607, 2619, 2631, 2643,
  2655, 2667, 2679, 2691, 2703, 2715, 2727, 2739, 2752, 2764,
  2776, 2788, 2800, 2812, 2824, 2836, 2848, 2860, 2872, 2884,
  2896, 2908, 2921, 2933, 2945, 2957, 2969, 2981, 2993, 3005,
  3017, 3029, 3041, 3053, 3065, 3077, 3090, 3077, 3065, 3053,
  3041, 3029, 3017, 3005, 2993, 2981, 2969, 2957, 2945, 2933,
  2921, 2908, 2896, 2884, 2872, 2860, 2848, 2836, 2824, 2812,
  2800, 2788, 2776, 2764, 2752, 2739, 2727, 2715, 2703, 2691,
  2679, 2667, 2655, 2643, 2631, 2619, 2607, 2595, 2583, 2570,
  2558, 2546, 2534, 2522, 2510, 2498, 2486, 2474, 2462, 2450,
  2438, 2426, 2414, 2401, 2389, 2377, 2365, 2353, 2341, 2329,
  2317, 2305, 2293, 2281, 2269, 2257, 2245, 2233, 2220, 2208,
  2196, 2184, 2172, 2160, 2148, 2136, 2124, 2112, 2100, 2088,
  2076, 2064, 2051, 2039, 2027, 2015, 2003, 1991, 1979, 1967,
  1955, 1943, 1931, 1919, 1907, 1895, 1882, 1870, 1858, 1846,
  1834, 1822, 1810, 1798, 1786, 1774, 1762, 1750, 1738, 1726,
  1713, 1701, 1689, 1677, 1665, 1653, 1641, 1629, 1617, 1605,
  1593, 1581, 1569, 1557, 1545, 1532, 1520, 1508, 1496, 1484,
  1472, 1460, 1448, 1436, 1424, 1412, 1400, 1388, 1376, 1363,
  1351, 1339, 1327, 1315, 1303, 1291, 1279, 1267, 1255, 1243,
  1231, 1219, 1207, 1194, 1182, 1170, 1158, 1146, 1134, 1122,
  1110, 1098, 1086, 1074, 1062, 1050, 1038, 1025, 1013, 1001,
  989, 977, 965, 953, 941, 929, 917, 905, 893, 881,
  869, 856, 844, 832, 820, 808, 796, 784, 772, 760,
  748, 736, 724, 712, 700, 688, 675, 663, 651, 639,
  627, 615, 603, 591, 579, 567, 555, 543, 531, 519,
  506, 494, 482, 470, 458, 446, 434, 422, 410, 398,
  386, 374, 362, 350, 337, 325, 313, 301, 289, 277,
  265, 253, 241, 229, 217, 205, 193, 181, 168, 156,
  144, 132, 120, 108, 96, 84, 72, 60, 48, 36,
  24, 12
};



// Exponential Rise LUT
const uint16_t expoRiseLUT[LUT_SIZE] = 
{
  0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 
  2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 
  5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 
  7, 7, 8, 8, 8, 9, 9, 9, 9, 10, 
  10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 
  13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 
  17, 17, 17, 18, 18, 19, 19, 19, 20, 20, 
  21, 21, 21, 22, 22, 23, 23, 24, 24, 24, 
  25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 
  30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 
  35, 35, 36, 36, 37, 38, 38, 39, 39, 40, 
  41, 41, 42, 42, 43, 44, 44, 45, 46, 46, 
  47, 48, 48, 49, 50, 50, 51, 52, 52, 53, 
  54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 
  62, 62, 63, 64, 65, 66, 67, 67, 68, 69, 
  70, 71, 72, 73, 74, 75, 75, 76, 77, 78, 
  79, 80, 81, 82, 83, 84, 85, 86, 88, 89, 
  90, 91, 92, 93, 94, 95, 96, 97, 99, 100, 
  101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 
  114, 115, 116, 118, 119, 120, 122, 123, 125, 126, 
  127, 129, 130, 132, 133, 135, 136, 138, 139, 141, 
  143, 144, 146, 148, 149, 151, 153, 154, 156, 158, 
  159, 161, 163, 165, 167, 169, 170, 172, 174, 176, 
  178, 180, 182, 184, 186, 188, 190, 192, 194, 196, 
  198, 201, 203, 205, 207, 209, 212, 214, 216, 219, 
  221, 223, 226, 228, 231, 233, 236, 238, 241, 243, 
  246, 249, 251, 254, 257, 259, 262, 265, 268, 270, 
  273, 276, 279, 282, 285, 288, 291, 294, 297, 300, 
  304, 307, 310, 313, 317, 320, 323, 327, 330, 333, 
  337, 340, 344, 348, 351, 355, 359, 362, 366, 370, 
  374, 378, 382, 385, 389, 394, 398, 402, 406, 410, 
  414, 419, 423, 427, 432, 436, 441, 445, 450, 454, 
  459, 464, 469, 473, 478, 483, 488, 493, 498, 503, 
  508, 514, 519, 524, 530, 535, 540, 546, 551, 557, 
  563, 569, 574, 580, 586, 592, 598, 604, 610, 617, 
  623, 629, 636, 642, 649, 655, 662, 668, 675, 682, 
  689, 696, 703, 710, 717, 725, 732, 739, 747, 754, 
  762, 770, 777, 785, 793, 801, 809, 817, 826, 834, 
  842, 851, 860, 868, 877, 886, 895, 904, 913, 922, 
  931, 941, 950, 960, 969, 979, 989, 999, 1009, 1019, 
  1029, 1039, 1050, 1060, 1071, 1082, 1093, 1104, 1115, 1126, 
  1137, 1148, 1160, 1172, 1183, 1195, 1207, 1219, 1231, 1244, 
  1256, 1269, 1281, 1294, 1307, 1320, 1333, 1347, 1360, 1374, 
  1387, 1401, 1415, 1429, 1444, 1458, 1472, 1487, 1502, 1517, 
  1532, 1547, 1563, 1578, 1594, 1610, 1626, 1642, 1659, 1675, 
  1692, 1709, 1726, 1743, 1760, 1778, 1795, 1813, 1831, 1849, 
  1868, 1886, 1905, 1924, 1943, 1962, 1982, 2002, 2022, 2042, 
  2062, 2082, 2103, 2124, 2145, 2166, 2188, 2210, 2232, 2254, 
  2276, 2299, 2321, 2344, 2368, 2391, 2415, 2439, 2463, 2487, 
  2512, 2537, 2562, 2588, 2613, 2639, 2665, 2692, 2718, 2745, 
  2773, 2800, 2828, 2856, 2884, 2913, 2941, 2971, 3000, 3030, 
  3060, 3090, 
};


// Exponential Fall LUT
const uint16_t expoFallLUT[LUT_SIZE] = 
{
  3090, 3060, 3030, 3000, 2971, 2941, 2913, 2884, 2856, 2828, 
  2800, 2773, 2745, 2718, 2692, 2665, 2639, 2613, 2588, 2562, 
  2537, 2512, 2487, 2463, 2439, 2415, 2391, 2368, 2344, 2321, 
  2299, 2276, 2254, 2232, 2210, 2188, 2166, 2145, 2124, 2103, 
  2082, 2062, 2042, 2022, 2002, 1982, 1962, 1943, 1924, 1905, 
  1886, 1868, 1849, 1831, 1813, 1795, 1778, 1760, 1743, 1726, 
  1709, 1692, 1675, 1659, 1642, 1626, 1610, 1594, 1578, 1563, 
  1547, 1532, 1517, 1502, 1487, 1472, 1458, 1444, 1429, 1415, 
  1401, 1387, 1374, 1360, 1347, 1333, 1320, 1307, 1294, 1281, 
  1269, 1256, 1244, 1231, 1219, 1207, 1195, 1183, 1172, 1160, 
  1148, 1137, 1126, 1115, 1104, 1093, 1082, 1071, 1060, 1050, 
  1039, 1029, 1019, 1009, 999, 989, 979, 969, 960, 950, 
  941, 931, 922, 913, 904, 895, 886, 877, 868, 860, 
  851, 842, 834, 826, 817, 809, 801, 793, 785, 777, 
  770, 762, 754, 747, 739, 732, 725, 717, 710, 703, 
  696, 689, 682, 675, 668, 662, 655, 649, 642, 636, 
  629, 623, 617, 610, 604, 598, 592, 586, 580, 574, 
  569, 563, 557, 551, 546, 540, 535, 530, 524, 519, 
  514, 508, 503, 498, 493, 488, 483, 478, 473, 469, 
  464, 459, 454, 450, 445, 441, 436, 432, 427, 423, 
  419, 414, 410, 406, 402, 398, 394, 389, 385, 382, 
  378, 374, 370, 366, 362, 359, 355, 351, 348, 344, 
  340, 337, 333, 330, 327, 323, 320, 317, 313, 310, 
  307, 304, 300, 297, 294, 291, 288, 285, 282, 279, 
  276, 273, 270, 268, 265, 262, 259, 257, 254, 251, 
  249, 246, 243, 241, 238, 236, 233, 231, 228, 226, 
  223, 221, 219, 216, 214, 212, 209, 207, 205, 203, 
  201, 198, 196, 194, 192, 190, 188, 186, 184, 182, 
  180, 178, 176, 174, 172, 170, 169, 167, 165, 163, 
  161, 159, 158, 156, 154, 153, 151, 149, 148, 146, 
  144, 143, 141, 139, 138, 136, 135, 133, 132, 130, 
  129, 127, 126, 125, 123, 122, 120, 119, 118, 116, 
  115, 114, 112, 111, 110, 108, 107, 106, 105, 103, 
  102, 101, 100, 99, 97, 96, 95, 94, 93, 92, 
  91, 90, 89, 88, 86, 85, 84, 83, 82, 81, 
  80, 79, 78, 77, 76, 75, 75, 74, 73, 72, 
  71, 70, 69, 68, 67, 67, 66, 65, 64, 63, 
  62, 62, 61, 60, 59, 58, 58, 57, 56, 55, 
  55, 54, 53, 52, 52, 51, 50, 50, 49, 48, 
  48, 47, 46, 46, 45, 44, 44, 43, 42, 42, 
  41, 41, 40, 39, 39, 38, 38, 37, 36, 36, 
  35, 35, 34, 34, 33, 33, 32, 32, 31, 31, 
  30, 30, 29, 29, 28, 28, 27, 27, 26, 26, 
  25, 25, 24, 24, 24, 23, 23, 22, 22, 21, 
  21, 21, 20, 20, 19, 19, 19, 18, 18, 17, 
  17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 
  14, 13, 13, 13, 12, 12, 12, 11, 11, 11, 
  10, 10, 10, 9, 9, 9, 9, 8, 8, 8, 
  7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 
  5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 
  2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 
  0, 0, 
};


// Inverted Exponential Rise LUT
const uint16_t invExpoRiseLUT[LUT_SIZE] = 
{
  0, 30, 60, 90, 119, 149, 177, 206, 234, 262, 
  290, 317, 345, 372, 398, 425, 451, 477, 502, 528, 
  553, 578, 603, 627, 651, 675, 699, 722, 746, 769, 
  791, 814, 836, 858, 880, 902, 924, 945, 966, 987, 
  1008, 1028, 1048, 1068, 1088, 1108, 1128, 1147, 1166, 1185, 
  1204, 1222, 1241, 1259, 1277, 1295, 1312, 1330, 1347, 1364, 
  1381, 1398, 1415, 1431, 1448, 1464, 1480, 1496, 1512, 1527, 
  1543, 1558, 1573, 1588, 1603, 1618, 1632, 1646, 1661, 1675, 
  1689, 1703, 1716, 1730, 1743, 1757, 1770, 1783, 1796, 1809, 
  1821, 1834, 1846, 1859, 1871, 1883, 1895, 1907, 1918, 1930, 
  1942, 1953, 1964, 1975, 1986, 1997, 2008, 2019, 2030, 2040, 
  2051, 2061, 2071, 2081, 2091, 2101, 2111, 2121, 2130, 2140, 
  2149, 2159, 2168, 2177, 2186, 2195, 2204, 2213, 2222, 2230, 
  2239, 2248, 2256, 2264, 2273, 2281, 2289, 2297, 2305, 2313, 
  2320, 2328, 2336, 2343, 2351, 2358, 2365, 2373, 2380, 2387, 
  2394, 2401, 2408, 2415, 2422, 2428, 2435, 2441, 2448, 2454, 
  2461, 2467, 2473, 2480, 2486, 2492, 2498, 2504, 2510, 2516, 
  2521, 2527, 2533, 2539, 2544, 2550, 2555, 2560, 2566, 2571, 
  2576, 2582, 2587, 2592, 2597, 2602, 2607, 2612, 2617, 2621, 
  2626, 2631, 2636, 2640, 2645, 2649, 2654, 2658, 2663, 2667, 
  2671, 2676, 2680, 2684, 2688, 2692, 2696, 2701, 2705, 2708, 
  2712, 2716, 2720, 2724, 2728, 2731, 2735, 2739, 2742, 2746, 
  2750, 2753, 2757, 2760, 2763, 2767, 2770, 2773, 2777, 2780, 
  2783, 2786, 2790, 2793, 2796, 2799, 2802, 2805, 2808, 2811, 
  2814, 2817, 2820, 2822, 2825, 2828, 2831, 2833, 2836, 2839, 
  2841, 2844, 2847, 2849, 2852, 2854, 2857, 2859, 2862, 2864, 
  2867, 2869, 2871, 2874, 2876, 2878, 2881, 2883, 2885, 2887, 
  2889, 2892, 2894, 2896, 2898, 2900, 2902, 2904, 2906, 2908, 
  2910, 2912, 2914, 2916, 2918, 2920, 2921, 2923, 2925, 2927, 
  2929, 2931, 2932, 2934, 2936, 2937, 2939, 2941, 2942, 2944, 
  2946, 2947, 2949, 2951, 2952, 2954, 2955, 2957, 2958, 2960, 
  2961, 2963, 2964, 2965, 2967, 2968, 2970, 2971, 2972, 2974, 
  2975, 2976, 2978, 2979, 2980, 2982, 2983, 2984, 2985, 2987, 
  2988, 2989, 2990, 2991, 2993, 2994, 2995, 2996, 2997, 2998, 
  2999, 3000, 3001, 3002, 3004, 3005, 3006, 3007, 3008, 3009, 
  3010, 3011, 3012, 3013, 3014, 3015, 3015, 3016, 3017, 3018, 
  3019, 3020, 3021, 3022, 3023, 3023, 3024, 3025, 3026, 3027, 
  3028, 3028, 3029, 3030, 3031, 3032, 3032, 3033, 3034, 3035, 
  3035, 3036, 3037, 3038, 3038, 3039, 3040, 3040, 3041, 3042, 
  3042, 3043, 3044, 3044, 3045, 3046, 3046, 3047, 3048, 3048, 
  3049, 3049, 3050, 3051, 3051, 3052, 3052, 3053, 3054, 3054, 
  3055, 3055, 3056, 3056, 3057, 3057, 3058, 3058, 3059, 3059, 
  3060, 3060, 3061, 3061, 3062, 3062, 3063, 3063, 3064, 3064, 
  3065, 3065, 3066, 3066, 3066, 3067, 3067, 3068, 3068, 3069, 
  3069, 3069, 3070, 3070, 3071, 3071, 3071, 3072, 3072, 3073, 
  3073, 3073, 3074, 3074, 3074, 3075, 3075, 3075, 3076, 3076, 
  3076, 3077, 3077, 3077, 3078, 3078, 3078, 3079, 3079, 3079, 
  3080, 3080, 3080, 3081, 3081, 3081, 3081, 3082, 3082, 3082, 
  3083, 3083, 3083, 3083, 3084, 3084, 3084, 3084, 3085, 3085, 
  3085, 3085, 3086, 3086, 3086, 3086, 3087, 3087, 3087, 3087, 
  3088, 3088, 3088, 3088, 3089, 3089, 3089, 3089, 3089, 3090, 
  3090, 3090, 
};



// Inverted Exponential Fall LUT
const uint16_t invExpoFallLUT[LUT_SIZE] = 
{
  3090, 3090, 3090, 3089, 3089, 3089, 3089, 3089, 3088, 3088, 
  3088, 3088, 3087, 3087, 3087, 3087, 3086, 3086, 3086, 3086, 
  3085, 3085, 3085, 3085, 3084, 3084, 3084, 3084, 3083, 3083, 
  3083, 3083, 3082, 3082, 3082, 3081, 3081, 3081, 3081, 3080, 
  3080, 3080, 3079, 3079, 3079, 3078, 3078, 3078, 3077, 3077, 
  3077, 3076, 3076, 3076, 3075, 3075, 3075, 3074, 3074, 3074, 
  3073, 3073, 3073, 3072, 3072, 3071, 3071, 3071, 3070, 3070, 
  3069, 3069, 3069, 3068, 3068, 3067, 3067, 3066, 3066, 3066, 
  3065, 3065, 3064, 3064, 3063, 3063, 3062, 3062, 3061, 3061, 
  3060, 3060, 3059, 3059, 3058, 3058, 3057, 3057, 3056, 3056, 
  3055, 3055, 3054, 3054, 3053, 3052, 3052, 3051, 3051, 3050, 
  3049, 3049, 3048, 3048, 3047, 3046, 3046, 3045, 3044, 3044, 
  3043, 3042, 3042, 3041, 3040, 3040, 3039, 3038, 3038, 3037, 
  3036, 3035, 3035, 3034, 3033, 3032, 3032, 3031, 3030, 3029, 
  3028, 3028, 3027, 3026, 3025, 3024, 3023, 3023, 3022, 3021, 
  3020, 3019, 3018, 3017, 3016, 3015, 3015, 3014, 3013, 3012, 
  3011, 3010, 3009, 3008, 3007, 3006, 3005, 3004, 3002, 3001, 
  3000, 2999, 2998, 2997, 2996, 2995, 2994, 2993, 2991, 2990, 
  2989, 2988, 2987, 2985, 2984, 2983, 2982, 2980, 2979, 2978, 
  2976, 2975, 2974, 2972, 2971, 2970, 2968, 2967, 2965, 2964, 
  2963, 2961, 2960, 2958, 2957, 2955, 2954, 2952, 2951, 2949, 
  2947, 2946, 2944, 2942, 2941, 2939, 2937, 2936, 2934, 2932, 
  2931, 2929, 2927, 2925, 2923, 2921, 2920, 2918, 2916, 2914, 
  2912, 2910, 2908, 2906, 2904, 2902, 2900, 2898, 2896, 2894, 
  2892, 2889, 2887, 2885, 2883, 2881, 2878, 2876, 2874, 2871, 
  2869, 2867, 2864, 2862, 2859, 2857, 2854, 2852, 2849, 2847, 
  2844, 2841, 2839, 2836, 2833, 2831, 2828, 2825, 2822, 2820, 
  2817, 2814, 2811, 2808, 2805, 2802, 2799, 2796, 2793, 2790, 
  2786, 2783, 2780, 2777, 2773, 2770, 2767, 2763, 2760, 2757, 
  2753, 2750, 2746, 2742, 2739, 2735, 2731, 2728, 2724, 2720, 
  2716, 2712, 2708, 2705, 2701, 2696, 2692, 2688, 2684, 2680, 
  2676, 2671, 2667, 2663, 2658, 2654, 2649, 2645, 2640, 2636, 
  2631, 2626, 2621, 2617, 2612, 2607, 2602, 2597, 2592, 2587, 
  2582, 2576, 2571, 2566, 2560, 2555, 2550, 2544, 2539, 2533, 
  2527, 2521, 2516, 2510, 2504, 2498, 2492, 2486, 2480, 2473, 
  2467, 2461, 2454, 2448, 2441, 2435, 2428, 2422, 2415, 2408, 
  2401, 2394, 2387, 2380, 2373, 2365, 2358, 2351, 2343, 2336, 
  2328, 2320, 2313, 2305, 2297, 2289, 2281, 2273, 2264, 2256, 
  2248, 2239, 2230, 2222, 2213, 2204, 2195, 2186, 2177, 2168, 
  2159, 2149, 2140, 2130, 2121, 2111, 2101, 2091, 2081, 2071, 
  2061, 2051, 2040, 2030, 2019, 2008, 1997, 1986, 1975, 1964, 
  1953, 1942, 1930, 1918, 1907, 1895, 1883, 1871, 1859, 1846, 
  1834, 1821, 1809, 1796, 1783, 1770, 1757, 1743, 1730, 1716, 
  1703, 1689, 1675, 1661, 1646, 1632, 1618, 1603, 1588, 1573, 
  1558, 1543, 1527, 1512, 1496, 1480, 1464, 1448, 1431, 1415, 
  1398, 1381, 1364, 1347, 1330, 1312, 1295, 1277, 1259, 1241, 
  1222, 1204, 1185, 1166, 1147, 1128, 1108, 1088, 1068, 1048, 
  1028, 1008, 987, 966, 945, 924, 902, 880, 858, 836, 
  814, 791, 769, 746, 722, 699, 675, 651, 627, 603, 
  578, 553, 528, 502, 477, 451, 425, 398, 372, 345, 
  317, 290, 262, 234, 206, 177, 149, 119, 90, 60, 
  30, 0, 
};





// Wobble Ramp Down LUT
const uint16_t wobRampDownLUT[LUT_SIZE] = 
{
  3088, 3088, 3088, 3088, 3088, 3087, 3087, 3086, 3085, 3084,
  3082, 3081, 3079, 3076, 3073, 3070, 3066, 3062, 3057, 3052,
  3046, 3040, 3033, 3026, 3018, 3009, 3000, 2990, 2980, 2969,
  2957, 2945, 2932, 2919, 2905, 2891, 2876, 2860, 2844, 2828,
  2811, 2793, 2775, 2757, 2739, 2720, 2700, 2681, 2661, 2641,
  2621, 2600, 2580, 2560, 2539, 2519, 2498, 2478, 2458, 2438,
  2419, 2399, 2380, 2362, 2343, 2326, 2308, 2292, 2276, 2260,
  2245, 2231, 2218, 2205, 2193, 2182, 2172, 2163, 2155, 2148,
  2141, 2136, 2132, 2128, 2126, 2125, 2125, 2126, 2128, 2132,
  2136, 2142, 2148, 2156, 2165, 2174, 2185, 2197, 2210, 2224,
  2239, 2254, 2271, 2288, 2307, 2326, 2346, 2366, 2387, 2409,
  2432, 2454, 2478, 2501, 2526, 2550, 2574, 2599, 2624, 2649,
  2674, 2698, 2723, 2747, 2771, 2795, 2818, 2840, 2862, 2884,
  2905, 2925, 2944, 2962, 2979, 2995, 3010, 3024, 3037, 3049,
  3059, 3068, 3075, 3081, 3085, 3088, 3090, 3089, 3087, 3083,
  3078, 3071, 3062, 3051, 3039, 3025, 3009, 2991, 2971, 2950,
  2927, 2902, 2875, 2846, 2816, 2784, 2751, 2715, 2679, 2640,
  2601, 2560, 2517, 2473, 2428, 2381, 2334, 2285, 2236, 2185,
  2134, 2082, 2029, 1976, 1922, 1867, 1813, 1758, 1703, 1648,
  1593, 1538, 1483, 1429, 1375, 1322, 1269, 1217, 1166, 1116,
  1067, 1019, 972, 926, 882, 840, 798, 759, 721, 685,
  651, 619, 589, 561, 535, 511, 490, 471, 454, 440,
  428, 418, 411, 407, 405, 406, 409, 415, 423, 434,
  448, 464, 482, 503, 527, 552, 581, 611, 644, 679,
  717, 756, 797, 841, 886, 933, 982, 1032, 1084, 1137,
  1192, 1248, 1305, 1363, 1422, 1481, 1541, 1601, 1661, 1719,
  1777, 1834, 1889, 1944, 1997, 2048, 2099, 2147, 2194, 2239,
  2282, 2323, 2362, 2400, 2434, 2467, 2498, 2526, 2551, 2575,
  2596, 2614, 2630, 2643, 2654, 2662, 2668, 2671, 2672, 2670,
  2666, 2659, 2649, 2637, 2623, 2606, 2587, 2566, 2542, 2516,
  2488, 2458, 2426, 2392, 2356, 2319, 2279, 2238, 2196, 2152,
  2107, 2060, 2012, 1963, 1913, 1862, 1810, 1758, 1705, 1651,
  1597, 1543, 1488, 1434, 1379, 1324, 1269, 1215, 1161, 1107,
  1054, 1002, 950, 899, 848, 799, 751, 704, 657, 612,
  569, 526, 485, 446, 408, 371, 336, 303, 271, 241,
  213, 186, 161, 138, 117, 98, 80, 64, 50, 37,
  27, 18, 11, 5, 2, 0, 0, 1, 4, 8,
  14, 21, 30, 40, 51, 64, 78, 93, 109, 126,
  144, 163, 183, 204, 225, 247, 269, 292, 315, 339,
  363, 388, 412, 437, 461, 486, 510, 534, 559, 582,
  606, 629, 651, 674, 695, 716, 736, 756, 775, 793,
  810, 826, 842, 857, 870, 883, 894, 905, 915, 923,
  931, 937, 942, 946, 950, 952, 953, 952, 951, 949,
  946, 941, 936, 930, 922, 914, 905, 895, 884, 872,
  859, 846, 832, 817, 802, 785, 769, 752, 734, 716,
  697, 678, 659, 640, 620, 600, 580, 560, 540, 519,
  499, 479, 459, 439, 419, 400, 380, 361, 343, 324,
  306, 289, 271, 255, 238, 223, 207, 193, 178, 165,
  152, 139, 127, 116, 105, 95, 86, 77, 69, 61,
  54, 47, 41, 35, 30, 26, 22, 18, 15, 12,
  10, 8, 6, 5, 4, 3, 2, 2, 1, 1,
  1, 1
};


// Wobble Ramp Up LUT
const uint16_t wobRampUpLUT[LUT_SIZE] = 
{
  2, 14, 26, 38, 50, 62, 73, 85, 96, 107,
  117, 128, 137, 147, 156, 165, 173, 181, 188, 195,
  202, 207, 213, 217, 221, 225, 228, 230, 232, 233,
  234, 233, 233, 232, 230, 227, 225, 221, 217, 213,
  208, 203, 197, 191, 184, 177, 170, 162, 155, 147,
  139, 131, 122, 114, 106, 97, 89, 81, 73, 65,
  57, 50, 43, 37, 31, 25, 20, 15, 11, 8,
  5, 3, 2, 1, 1, 2, 4, 7, 11, 16,
  22, 29, 36, 45, 55, 66, 78, 91, 106, 121,
  137, 155, 174, 193, 214, 236, 259, 283, 308, 334,
  360, 388, 417, 447, 477, 508, 540, 573, 606, 640,
  674, 709, 745, 780, 816, 853, 889, 926, 963, 1000,
  1037, 1074, 1110, 1146, 1182, 1218, 1253, 1288, 1322, 1356,
  1389, 1421, 1452, 1482, 1511, 1540, 1567, 1593, 1618, 1641,
  1663, 1684, 1704, 1722, 1738, 1753, 1767, 1778, 1788, 1797,
  1803, 1808, 1812, 1813, 1813, 1810, 1807, 1801, 1793, 1784,
  1773, 1760, 1745, 1729, 1711, 1691, 1669, 1646, 1622, 1595,
  1568, 1539, 1508, 1476, 1443, 1409, 1374, 1337, 1300, 1261,
  1222, 1182, 1141, 1100, 1058, 1016, 973, 931, 888, 845,
  802, 759, 717, 674, 633, 591, 551, 511, 472, 434,
  397, 361, 326, 293, 261, 230, 201, 174, 148, 124,
  102, 82, 64, 48, 34, 23, 13, 6, 2, 0,
  0, 2, 7, 15, 25, 38, 53, 71, 92, 115,
  140, 168, 199, 232, 267, 305, 346, 388, 433, 481,
  530, 581, 635, 690, 748, 807, 867, 930, 994, 1059,
  1126, 1194, 1263, 1333, 1404, 1475, 1548, 1620, 1691, 1762,
  1831, 1900, 1968, 2034, 2099, 2163, 2225, 2286, 2345, 2402,
  2457, 2510, 2561, 2611, 2657, 2702, 2745, 2785, 2823, 2858,
  2891, 2922, 2949, 2975, 2998, 3018, 3036, 3051, 3064, 3074,
  3082, 3087, 3089, 3090, 3087, 3083, 3076, 3066, 3055, 3041,
  3025, 3007, 2987, 2965, 2942, 2916, 2889, 2860, 2830, 2798,
  2764, 2730, 2694, 2657, 2619, 2581, 2541, 2501, 2460, 2418,
  2376, 2334, 2291, 2249, 2206, 2163, 2121, 2079, 2037, 1995,
  1954, 1914, 1874, 1835, 1797, 1759, 1723, 1688, 1654, 1621,
  1590, 1559, 1530, 1503, 1477, 1453, 1430, 1408, 1389, 1371,
  1355, 1340, 1327, 1316, 1307, 1300, 1294, 1290, 1288, 1288,
  1289, 1293, 1298, 1304, 1313, 1323, 1335, 1348, 1363, 1379,
  1397, 1417, 1437, 1460, 1483, 1508, 1534, 1561, 1589, 1618,
  1648, 1679, 1711, 1744, 1777, 1811, 1846, 1881, 1916, 1952,
  1988, 2024, 2061, 2097, 2134, 2171, 2207, 2244, 2280, 2316,
  2351, 2386, 2421, 2455, 2489, 2522, 2554, 2586, 2617, 2647,
  2676, 2705, 2732, 2759, 2784, 2809, 2833, 2856, 2877, 2898,
  2917, 2936, 2953, 2969, 2985, 2999, 3012, 3024, 3035, 3044,
  3053, 3061, 3068, 3073, 3078, 3082, 3085, 3087, 3088, 3088,
  3087, 3086, 3084, 3081, 3078, 3074, 3070, 3064, 3059, 3053,
  3046, 3040, 3032, 3025, 3017, 3010, 3002, 2994, 2985, 2977,
  2969, 2961, 2953, 2945, 2937, 2930, 2923, 2916, 2909, 2903,
  2897, 2891, 2886, 2882, 2877, 2874, 2871, 2868, 2866, 2864,
  2863, 2863, 2863, 2864, 2865, 2867, 2870, 2873, 2877, 2881,
  2886, 2891, 2897, 2904, 2911, 2918, 2926, 2935, 2944, 2953,
  2963, 2973, 2983, 2994, 3005, 3016, 3028, 3039, 3051, 3063,
  3075, 3087
};



// stepUp4 LUT
const uint16_t stepUp4LUT[LUT_SIZE] = 
{
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 775, 787, 800, 812, 824, 836,
  848, 860, 872, 884, 896, 908, 920, 933, 945, 957,
  969, 981, 993, 1005, 1017, 1029, 1041, 1053, 1066, 1078,
  1090, 1102, 1114, 1126, 1138, 1150, 1162, 1174, 1186, 1198,
  1211, 1223, 1235, 1247, 1259, 1271, 1283, 1295, 1307, 1319,
  1331, 1344, 1356, 1368, 1380, 1392, 1404, 1416, 1428, 1440,
  1452, 1464, 1477, 1489, 1501, 1513, 1525, 1537, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 2323, 2335, 2347, 2359, 2371, 2383, 2395, 2407,
  2419, 2431, 2444, 2456, 2468, 2480, 2492, 2504, 2516, 2528,
  2540, 2552, 2564, 2577, 2589, 2601, 2613, 2625, 2637, 2649,
  2661, 2673, 2685, 2697, 2709, 2722, 2734, 2746, 2758, 2770,
  2782, 2794, 2806, 2818, 2830, 2842, 2855, 2867, 2879, 2891,
  2903, 2915, 2927, 2939, 2951, 2963, 2975, 2988, 3000, 3012,
  3024, 3036, 3048, 3060, 3072, 3084, 5, 17, 29, 41,
  53, 65, 77, 89, 101, 114, 126, 138, 150, 162,
  174, 186, 198, 210, 222, 234, 247, 259, 271, 283,
  295, 307, 319, 331, 343, 355, 367, 380, 392, 404,
  416, 428, 440, 452, 464, 476, 488, 500, 512, 525,
  537, 549, 561, 573, 585, 597, 609, 621, 633, 645,
  658, 670, 682, 694, 706, 718, 730, 742, 754, 766,
  3090, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 1552, 1564, 1576, 1588, 1600, 1612,
  1625, 1637, 1649, 1661, 1673, 1685, 1697, 1709, 1721, 1733,
  1745, 1758, 1770, 1782, 1794, 1806, 1818, 1830, 1842, 1854,
  1866, 1878, 1891, 1903, 1915, 1927, 1939, 1951, 1963, 1975,
  1987, 1999, 2011, 2023, 2036, 2048, 2060, 2072, 2084, 2096,
  2108, 2120, 2132, 2144, 2156, 2169, 2181, 2193, 2205, 2217,
  2229, 2241, 2253, 2265, 2277, 2289, 2302, 2314, 3088, 3088,
  3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088,
  3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088,
  3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088,
  3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088,
  3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087,
  3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087,
  3087, 3087
};


// stepDown4 LUT
const uint16_t stepDown4LUT[LUT_SIZE] = 
{
  3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087,
  3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087, 3087,
  3087, 3087, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088,
  3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088,
  3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088,
  3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088, 3088,
  3088, 3088, 3088, 3088, 2314, 2302, 2289, 2277, 2265, 2253,
  2241, 2229, 2217, 2205, 2193, 2181, 2169, 2156, 2144, 2132,
  2120, 2108, 2096, 2084, 2072, 2060, 2048, 2036, 2023, 2011,
  1999, 1987, 1975, 1963, 1951, 1939, 1927, 1915, 1903, 1891,
  1878, 1866, 1854, 1842, 1830, 1818, 1806, 1794, 1782, 1770,
  1758, 1745, 1733, 1721, 1709, 1697, 1685, 1673, 1661, 1649,
  1637, 1625, 1612, 1600, 1588, 1576, 1564, 1552, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089, 3089,
  3089, 3090, 766, 754, 742, 730, 718, 706, 694, 682,
  670, 658, 645, 633, 621, 609, 597, 585, 573, 561,
  549, 537, 525, 512, 500, 488, 476, 464, 452, 440,
  428, 416, 404, 392, 380, 367, 355, 343, 331, 319,
  307, 295, 283, 271, 259, 247, 234, 222, 210, 198,
  186, 174, 162, 150, 138, 126, 114, 101, 89, 77,
  65, 53, 41, 29, 17, 5, 3084, 3072, 3060, 3048,
  3036, 3024, 3012, 3000, 2988, 2975, 2963, 2951, 2939, 2927,
  2915, 2903, 2891, 2879, 2867, 2855, 2842, 2830, 2818, 2806,
  2794, 2782, 2770, 2758, 2746, 2734, 2722, 2709, 2697, 2685,
  2673, 2661, 2649, 2637, 2625, 2613, 2601, 2589, 2577, 2564,
  2552, 2540, 2528, 2516, 2504, 2492, 2480, 2468, 2456, 2444,
  2431, 2419, 2407, 2395, 2383, 2371, 2359, 2347, 2335, 2323,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 1537, 1525, 1513, 1501, 1489, 1477,
  1464, 1452, 1440, 1428, 1416, 1404, 1392, 1380, 1368, 1356,
  1344, 1331, 1319, 1307, 1295, 1283, 1271, 1259, 1247, 1235,
  1223, 1211, 1198, 1186, 1174, 1162, 1150, 1138, 1126, 1114,
  1102, 1090, 1078, 1066, 1053, 1041, 1029, 1017, 1005, 993,
  981, 969, 957, 945, 933, 920, 908, 896, 884, 872,
  860, 848, 836, 824, 812, 800, 787, 775, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2
};




#endif
