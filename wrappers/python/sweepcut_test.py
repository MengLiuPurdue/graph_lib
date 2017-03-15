from python_wrapper import python_wrapper
f = open("minnesota.smat")
first_line = f.readline().strip()
import re
first_line = re.split(" ",first_line)
n = long(first_line[0])
m = long(first_line[2])
ei = []
ej = []
w = []
    
data = f.read()
data = data.split()
for i in range(m):
    ei += [int(data[3 * i])]
    ej += [int(data[3 * i + 1])]
    w += [data[3 * i + 2]]
    
f.close()


ids=[220,613,1610,65,158,229,305,467,608,742,753,953,1506,1516,1574,2154,2251,2428,82,206,211,218,252,364,370,393,433,437,499,504,513,562,571,587,617,640,654,655,686,691,694,702,714,718,767,879,899,903,1002,1167,1238,1281,1298,1311,1319,1322,1338,1360,1365,1416,1436,1450,1468,1479,1484,1542,1593,1605,1691,1719,1782,1798,1800,1839,1854,1896,1954,2005,2023,2027,2029,2033,2073,2111,2137,2146,2148,2163,2167,2196,2252,2290,2331,2337,2338,2361,2369,2370,2375,2397,2435,2459,2492,2497,4,46,101,150,154,237,295,344,345,391,430,469,496,502,521,566,631,711,716,772,978,980,1026,1131,1133,1219,1359,1362,1389,1534,1537,1541,1581,1617,1619,1712,1733,1761,1778,1812,2094,2109,2123,2175,2237,2334,2376,2415,1,3,18,25,37,40,57,58,59,94,96,97,106,108,110,114,115,126,135,137,145,152,153,156,182,188,200,208,210,222,233,234,243,247,263,265,276,286,308,312,333,349,383,395,397,411,431,443,465,470,473,480,510,544,563,568,569,579,586,590,623,629,642,643,648,650,657,667,670,679,685,697,706,715,730,743,752,766,768,774,776,779,796,799,805,807,816,825,832,837,852,859,872,890,900,910,932,940,976,995,996,1000,1012,1046,1050,1055,1070,1086,1087,1094,1097,1101,1103,1105,1123,1147,1154,1179,1191,1240,1285,1292,1308,1310,1346,1349,1378,1407,1419,1424,1438,1441,1445,1471,1485,1492,1497,1503,1521,1525,1540,1548,1556,1558,1563,1570,1571,1583,1585,1587,1600,1621,1624,1644,1645,1646,1655,1659,1667,1682,1707,1709,1756,1764,1771,1781,1794,1801,1804,1816,1820,1831,1832,1834,1864,1876,1878,1881,1883,1886,1888,1892,1902,1919,1923,1934,1936,1938,1948,1950,1961,1973,2012,2015,2041,2048,2055,2056,2062,2065,2087,2095,2106,2124,2127,2128,2131,2149,2156,2180,2211,2214,2224,2231,2249,2260,2266,2281,2308,2320,2323,2326,2345,2350,2351,2357,2358,2395,2398,2408,2449,2451,2453,2466,2467,2509,2519,2521,2527,2542,2547,2550,2551,2556,2558,2564,2576,2582,2589,2605,2606,2607,2626,2635,2638,6,15,21,22,29,32,39,42,45,56,67,69,81,83,88,98,100,103,105,111,113,116,124,141,148,167,175,205,216,219,221,223,224,231,232,238,240,242,246,257,272,280,302,309,311,315,317,327,329,330,338,341,343,354,365,373,385,396,398,423,424,428,445,449,450,452,466,468,477,479,489,492,500,503,505,507,511,512,520,522,528,533,554,555,559,561,567,576,588,602,606,607,609,610,616,619,627,633,635,636,637,638,649,661,665,666,671,677,682,693,696,705,707,708,720,723,727,732,733,734,758,759,761,780,783,785,787,801,803,804,808,810,811,812,813,830,833,841,842,851,871,888,906,908,909,911,914,915,923,927,931,934,939,941,948,949,952,959,969,973,989,990,991,992,998,1004,1015,1018,1021,1024,1034,1035,1045,1049,1057,1074,1107,1115,1121,1126,1129,1140,1143,1144,1145,1149,1152,1153,1158,1160,1162,1168,1176,1177,1178,1188,1196,1200,1201,1209,1210,1215,1223,1228,1241,1249,1269,1270,1284,1293,1294,1296,1297,1301,1305,1307,1316,1318,1327,1328,1329,1336,1337,1351,1352,1353,1361,1366,1369,1370,1375,1380,1381,1385,1391,1399,1405,1408,1409,1412,1414,1421,1423,1425,1428,1435,1440,1442,1446,1453,1456,1459,1462,1464,1466,1475,1487,1491,1495,1499,1527,1536,1547,1553,1564,1569,1572,1576,1578,1582,1584,1590,1597,1599,1607,1608,1609,1611,1613,1615,1616,1627,1629,1630,1638,1640,1643,1649,1664,1666,1673,1685,1687,1692,1693,1696,1698,1701,1721,1729,1731,1740,1742,1744,1749,1751,1753,1754,1757,1760,1769,1770,1775,1779,1780,1793,1795,1802,1811,1814,1817,1823,1827,1837,1849,1850,1852,1853,1855,1866,1868,1880,1882,1890,1893,1895,1901,1905,1907,1908,1909,1921,1929,1933,1937,1942,1949,1959,1960,1964,1968,1969,1974,1978,1980,1981,1988,1989,1997,2013,2014,2017,2018,2024,2030,2031,2035,2038,2039,2046,2051,2052,2059,2066,2079,2080,2085,2105,2108,2110,2113,2122,2129,2139,2160,2164,2165,2166,2183,2184,2186,2198,2199,2200,2206,2207,2213,2216,2229,2230,2234,2240,2244,2246,2265,2274,2276,2284,2289,2304,2305,2318,2328,2333,2339,2347,2359,2362,2364,2377,2384,2391,2394,2396,2412,2416,2418,2420,2421,2445,2446,2448,2450,2461,2463,2465,2473,2475,2477,2479,2481,2485,2495,2498,2499,2523,2526,2532,2535,2536,2539,2543,2554,2572,2573,2575,2577,2578,2580,2587,2588,2594,2598,2604,2609,2615,2616,2617,2618,2620,2622,2630,11,14,16,19,44,47,49,60,61,66,72,80,89,90,95,107,109,125,128,140,146,147,149,162,166,174,183,187,189,190,193,196,198,201,204,228,230,244,249,251,267,270,278,294,316,321,326,328,342,359,363,366,371,380,387,388,392,402,416,418,441,446,456,463,476,516,519,523,527,534,537,538,539,540,551,553,558,573,582,584,593,598,621,634,644,647,660,678,681,683,684,725,735,748,755,757,781,831,835,839,840,854,864,865,882,886,904,912,960,982,983,1008,1014,1016,1019,1031,1058,1073,1082,1088,1089,1093,1096,1100,1111,1112,1122,1130,1134,1136,1148,1150,1173,1181,1182,1192,1197,1204,1205,1208,1232,1236,1247,1253,1267,1277,1288,1306,1312,1330,1348,1354,1357,1376,1379,1388,1398,1415,1427,1430,1431,1432,1458,1470,1474,1476,1482,1483,1488,1504,1507,1514,1519,1524,1539,1550,1561,1573,1579,1580,1614,1641,1674,1677,1694,1704,1706,1713,1714,1715,1723,1734,1735,1736,1737,1755,1765,1783,1784,1786,1788,1790,1806,1807,1818,1819,1821,1822,1835,1836,1840,1842,1873,1904,1911,1924,1928,1935,1939,1940,1952,1955,1956,1982,1987,2000,2019,2036,2044,2049,2050,2060,2063,2064,2068,2092,2096,2114,2118,2121,2125,2133,2136,2144,2162,2190,2195,2197,2215,2221,2226,2236,2239,2255,2263,2264,2268,2271,2273,2282,2286,2288,2296,2301,2310,2319,2344,2348,2356,2371,2374,2400,2404,2405,2413,2414,2417,2423,2432,2444,2488,2489,2512,2522,2561,2585,2590,2611,2613,2614,2619,2629,2631,2634,2641,0,2,7,12,13,20,26,27,28,31,34,35,36,41,50,52,55,62,64,68,77,78,79,85,87,92,99,102,112,117,118,120,122,123,130,132,136,139,144,165,169,171,173,177,180,181,184,186,195,199,202,207,209,212,213,214,215,225,235,236,241,245,250,254,259,260,261,262,264,266,268,269,273,274,275,277,279,282,283,284,285,290,293,304,307,320,332,336,339,340,356,360,369,372,374,375,376,378,379,381,382,384,386,389,390,394,406,407,408,413,414,419,421,426,434,438,440,442,444,448,454,457,471,472,475,482,493,494,497,498,506,514,517,525,526,531,535,536,543,546,547,549,564,570,583,585,591,599,601,604,605,611,612,615,618,622,625,628,639,646,659,662,663,669,673,676,680,689,698,699,700,701,703,710,713,717,722,728,731,737,739,740,741,754,762,763,765,777,778,786,790,791,792,794,800,815,820,826,834,843,844,845,849,857,858,861,863,866,868,873,878,881,883,889,898,902,905,913,916,918,920,924,938,942,944,946,954,956,961,966,968,972,986,987,993,999,1001,1007,1010,1013,1020,1023,1025,1028,1030,1032,1033,1036,1037,1038,1042,1043,1048,1051,1059,1061,1065,1071,1075,1076,1077,1078,1091,1099,1104,1108,1110,1113,1117,1118,1119,1125,1127,1135,1139,1141,1151,1155,1163,1164,1165,1169,1171,1172,1174,1184,1187,1195,1199,1202,1207,1212,1217,1218,1221,1222,1227,1235,1237,1239,1244,1245,1248,1251,1252,1254,1256,1257,1259,1260,1262,1264,1266,1272,1275,1279,1283,1287,1289,1295,1299,1300,1303,1315,1317,1323,1324,1326,1333,1339,1340,1345,1347,1355,1356,1363,1364,1367,1368,1392,1396,1406,1410,1413,1417,1418,1422,1426,1433,1444,1449,1455,1460,1465,1467,1469,1473,1477,1481,1489,1498,1502,1508,1510,1528,1529,1538,1546,1552,1557,1559,1568,1577,1589,1594,1596,1598,1603,1620,1623,1628,1633,1634,1635,1637,1639,1652,1653,1657,1661,1662,1670,1678,1683,1686,1689,1695,1699,1700,1705,1708,1710,1711,1720,1722,1724,1726,1727,1728,1732,1738,1745,1746,1748,1750,1758,1759,1762,1763,1772,1776,1785,1789,1791,1792,1797,1803,1805,1810,1813,1815,1825,1828,1830,1845,1846,1848,1856,1863,1865,1869,1874,1875,1877,1879,1887,1889,1899,1903,1906,1910,1918,1920,1922,1926,1927,1932,1941,1944,1945,1946,1947,1962,1963,1971,1977,1983,1984,1985,1993,1994,1998,2009,2021,2022,2025,2037,2045,2054,2061,2071,2074,2076,2077,2078,2088,2098,2099,2102,2104,2112,2116,2119,2120,2126,2130,2132,2134,2142,2143,2147,2158,2161,2168,2170,2176,2181,2189,2191,2192,2193,2201,2205,2210,2212,2218,2222,2223,2227,2232,2235,2241,2245,2262,2267,2269,2270,2277,2279,2285,2293,2309,2311,2313,2316,2321,2327,2332,2343,2349,2354,2360,2363,2367,2380,2381,2382,2386,2390,2392,2393,2401,2402,2406,2422,2427,2430,2434,2438,2454,2458,2468,2470,2471,2476,2478,2484,2490,2494,2500,2501,2504,2507,2511,2513,2514,2516,2517,2520,2525,2529,2545,2546,2549,2552,2555,2559,2565,2566,2569,2584,2591,2608,2623,2627,2628,2632,2633,17,23,24,30,38,43,48,51,53,63,71,73,84,91,93,134,138,142,143,170,172,226,291,296,301,313,314,322,323,346,347,348,351,355,368,377,399,400,425,429,432,435,436,453,455,458,459,460,478,481,484,486,490,495,501,508,529,530,541,542,550,552,560,565,574,577,580,589,592,595,597,614,624,630,641,645,656,658,672,674,695,704,724,745,746,751,760,770,771,788,793,797,798,802,806,814,822,827,838,847,853,855,856,860,867,869,874,877,880,885,893,894,895,896,897,907,917,922,929,935,936,937,943,947,957,958,963,967,970,975,981,985,997,1022,1039,1040,1044,1053,1054,1064,1079,1083,1090,1095,1102,1116,1120,1124,1137,1138,1146,1183,1193,1198,1206,1211,1213,1226,1246,1250,1261,1263,1268,1273,1280,1282,1291,1302,1304,1314,1320,1321,1325,1332,1334,1343,1372,1374,1382,1397,1400,1401,1402,1403,1434,1447,1452,1454,1461,1478,1480,1494,1501,1511,1512,1513,1520,1526,1533,1543,1544,1551,1554,1562,1565,1567,1575,1586,1591,1592,1625,1631,1632,1636,1642,1650,1651,1654,1656,1663,1675,1676,1681,1684,1725,1766,1768,1787,1799,1809,1841,1847,1851,1857,1859,1860,1871,1894,1897,1912,1913,1916,1970,1972,1979,1986,1991,1996,2001,2003,2006,2007,2008,2026,2028,2040,2043,2067,2069,2070,2075,2082,2084,2089,2093,2097,2100,2115,2140,2151,2152,2153,2157,2173,2188,2208,2209,2219,2228,2238,2247,2248,2250,2253,2256,2257,2258,2261,2278,2291,2294,2300,2303,2307,2314,2315,2317,2322,2330,2336,2341,2372,2378,2379,2383,2389,2399,2407,2411,2424,2429,2431,2437,2440,2452,2456,2462,2493,2496,2503,2505,2508,2510,2515,2528,2530,2533,2537,2544,2548,2557,2562,2567,2568,2570,2571,2581,2583,2586,2592,2593,2596,2599,2603,2610,2621,2624,2636,2637,5,9,33,54,74,76,86,104,119,121,127,129,131,133,157,159,163,168,176,178,185,192,194,227,239,253,256,258,271,281,288,289,297,298,299,303,310,318,319,325,331,337,352,357,361,367,404,405,412,415,427,439,461,474,483,487,488,491,509,515,524,548,557,572,575,581,594,596,600,626,651,652,653,664,675,688,690,692,709,712,719,721,726,729,738,747,750,756,773,782,789,795,809,817,819,821,823,828,829,846,848,850,875,891,892,901,919,925,928,933,955,964,965,977,984,988,1003,1005,1009,1017,1027,1041,1056,1063,1066,1068,1072,1080,1081,1084,1085,1092,1098,1106,1114,1157,1159,1161,1166,1170,1175,1185,1186,1189,1190,1203,1214,1216,1225,1229,1231,1233,1242,1255,1258,1271,1274,1278,1286,1290,1313,1331,1335,1341,1350,1358,1373,1383,1384,1386,1387,1404,1411,1420,1429,1437,1439,1443,1448,1457,1463,1472,1486,1490,1493,1496,1500,1509,1515,1517,1518,1522,1531,1532,1545,1549,1555,1560,1566,1588,1595,1601,1602,1604,1606,1612,1618,1622,1626,1647,1658,1660,1668,1669,1702,1703,1718,1730,1739,1741,1743,1747,1752,1767,1773,1774,1824,1826,1829,1843,1861,1862,1867,1884,1885,1891,1898,1900,1925,1930,1943,1953,1957,1958,1965,1967,1976,1990,2002,2004,2010,2011,2032,2042,2053,2057,2058,2072,2081,2083,2086,2091,2103,2107,2117,2141,2145,2155,2171,2174,2177,2178,2179,2182,2185,2187,2202,2204,2217,2220,2233,2242,2243,2254,2275,2287,2297,2299,2306,2312,2324,2325,2335,2340,2352,2355,2366,2368,2373,2387,2403,2409,2410,2419,2436,2439,2441,2442,2447,2457,2460,2464,2472,2474,2482,2483,2486,2491,2524,2531,2534,2538,2540,2541,2553,2563,2574,2579,2597,2600,2625,2639,2640,10,155,179,248,292,324,334,335,350,362,417,451,462,603,632,784,950,979,994,1006,1060,1062,1067,1069,1142,1156,1224,1243,1309,1393,1535,1688,1716,1796,1833,1844,1872,1914,1915,1931,1966,1975,1992,1999,2034,2047,2090,2101,2169,2172,2194,2272,2283,2292,2342,2346,2353,2365,2426,2506,2560,2602,2612,8,70,75,151,161,164,191,197,217,255,287,300,353,358,401,403,410,420,518,545,556,578,668,687,736,744,749,764,769,775,824,836,862,870,884,887,921,926,945,962,974,1011,1029,1128,1132,1180,1220,1230,1234,1265,1276,1342,1344,1371,1377,1390,1451,1523,1530,1665,1672,1679,1680,1690,1717,1777,1808,1838,1870,1917,1951,1995,2016,2020,2150,2225,2259,2295,2385,2388,2425,2469,2480,2487,2502,2595,2601,306,409,422,447,464,485,532,818,876,951,971,1047,1052,1109,1194,1395,1505,1648,1671,1697,1858,2138,2159,2203,2280,2298,2302,2329,2443,2455,2518,160,203,620,2135,2433,930,1394]

values=[]
num=2642
fun_id=(0,1)
(actual_length,results)=python_wrapper(n,ei,ej,ids,num,values,fun_id)
print actual_length,results
