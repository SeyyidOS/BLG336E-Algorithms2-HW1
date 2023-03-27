import matplotlib.pyplot as plt

# define data points
child_numbers = [7, 10, 20, 49, 191, 376, 821, 1260]
runtimes = [1218, 1365, 1378, 2147, 11423, 36752, 106593, 202773]

# plot the line chart
plt.plot(child_numbers, runtimes, '-o', linewidth=2, markersize=8)

# set axis labels and title
plt.xlabel('Child Number')
plt.ylabel('Runtime (microseconds)')
plt.title('Runtime results for different input cases')

# show the chart
plt.show()