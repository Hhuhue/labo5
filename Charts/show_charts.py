import json
import matplotlib.pyplot as plt

def showCharts(data):
    row = 3
    col = 3
    plt.inferno()
    figure, axes = plt.subplots(row, col)
    plots = []
    for i in range(int(len(data))):
        axis = axes[int(i/col)][i%col]
        dataset = data[i]
        ratios = [dataset["Original_time"]/t for t in dataset["Times"]]
        plot = axis.plot(dataset["Targets"], ratios)
        axis.set_ylim(0, 1.05)
        if(i == 0):
            plots.append(plot)
        axis.set_xlabel("Demanded Frequency (kHz)")
        axis.set_ylabel("Performance Ratio")
        axis.set_title(str(dataset["Title"] + " (" + str(dataset["Freq"]) + "kHz)"))
        axis.grid(True)

    #figure.legend(plots, loc="center right", borderaxespad=0.1, title="Native Frequencies type")
    figure.tight_layout(pad=0.1)   
    plt.subplots_adjust(right=0.85)
    plt.show()


def showChart(data):
    row = 1
    col = 1
    plt.inferno()
    figure, axis = plt.subplots(row, col)
    plots = []
    legend_labels = []
    for i in range(int(len(data))):
        dataset = data[i]
        ratios = [dataset["Original_time"]/t for t in dataset["Times"]]
        plot = axis.plot(dataset["Targets"], ratios)
        axis.set_ylim(0, 1.05)
        plots.append(plot)
        legend_labels.append(str(dataset["Title"] + " (" + str(dataset["Freq"]) + "kHz)"))

    axis.set_xlabel("Requested Frequency (kHz)")
    axis.set_ylabel("Performance Ratio")
    axis.set_title("Performance Ratio by Requested Frequencies")
    axis.grid(True)

    plt.legend(plots, labels=legend_labels, loc="upper right", prop={'size':8}, framealpha=1.0, borderaxespad=0.1, title="Native Frequencies")
    figure.tight_layout(pad=0.1)   
    plt.subplots_adjust(right=0.85)
    plt.show()

file = open("Times.json", "r")
content = file.read() 
decoder = json.JSONDecoder()      
data = decoder.decode(content)

showChart(data)
