# I started playing speed chess on chess.com and drank while I did it
# This is the natural decline of my rating! Just as expected lol

import matplotlib.pyplot as plt
import numpy as np

times = [1054, 1046, 1053, 1060, 1066, 1058, 1065, 1073, 1079, 1088, 1079, 1072, 1064, 1073, 1082, 1073, 1083, 1090, 1096, 1089, 1080, 1072, 1079, 1074, 1065, 1059, 1050, 1057, 1051, 1046, 1055, 1063, 1056, 1048, 1054, 1064, 1056, 1047, 1041, 1049, 1040, 1032, 1023, 1029, 1034, 1042, 1031, 1024, 1019, 1027, 1019, 1026, 1019, 1027, 1037, 1030, 1039, 1030, 1022, 1031, 1022, 1014, 1024, 1016, 1024, 1029, 1037, 1026, 1020, 1010, 1017, 1008, 1016, 1010, 1004, 1012, 1003, 1010, 1001, 1009, 1002, 994, 1000, 1007, 998, 992, 985, 976]
bottom = np.arange(0, len(times), 1)

plt.plot(bottom, times)

plt.xlabel("games")
plt.ylabel("rating")
plt.title("Chess Ranking Plummet while Drinking")
plt.grid(True)
plt.show()
