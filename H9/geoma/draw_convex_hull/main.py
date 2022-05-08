import numpy as np
import matplotlib.pyplot as plt

ps = [[9.8349609375, -2.578125], [-8.015625, -0.609375], [-5.734375, -3.1875]]
ch = [[-5.734375, -3.1875], [9.8349609375, -2.578125], [-8.015625, -0.609375]]

ps = np.array (ps).T
ch = np.array (ch).T

plt.scatter (ps[0], ps[1])
# plt.plot (ps[0], ps[1])
# plt.scatter (ch[0], ch[1], color='r', s=2)
plt.plot (ch[0], ch[1], color='r')

plt.show ()