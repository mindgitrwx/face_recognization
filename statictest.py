import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.legend as legend
import numpy as np
import time
import sys
import os
import MySQLdb


def timeclassify(string):
    newstr = string.split('_')
    hour = int(newstr[3])
    
    if 0 <= hour < 6:
        return 0
    elif 6 <= hour < 12:
        return 1
    elif 12 <= hour < 18:
        return 2
    elif 18 <= hour < 24:
        return 3
    else:
        return -1


#print(timeparser(str))


def fileName():
    dte = time.localtime()
    Year = dte.tm_year
    Mon = dte.tm_mon
    Day = dte.tm_mday
    WDay = dte.tm_wday
    Hour = dte.tm_hour
    Min = dte.tm_min
    Sec = dte.tm_sec
    imgFileName = str(Year) + '_' + str(Mon) + '_' + str(Day) + '.png'
    return imgFileName



timeline = ['00:00~06:00','06:00~12:00','12:00~18:00','18:00~24:00']



N = 4


happy = [5,2,4,5]
neutral  = [6,7,9,10]
sad = [1,0,4,3]
angry = [2,3,0,1]





baseforsad = [happy[0]+neutral[0],happy[1]+neutral[1],happy[2]+neutral[2],happy[3]+neutral[3]]
baseforangry =[baseforsad[0]+sad[0],baseforsad[1]+sad[1],baseforsad[2]+sad[2],baseforsad[3]+sad[3]]

ind = np.arange(N)    # the x locations for the groups
width = 0.35       # the width of the bars: can also be len(x) sequence

p1 = plt.bar(ind, happy, width, color='y')
p2 = plt.bar(ind, neutral, width, bottom=happy, color='g')
p3 = plt.bar(ind, sad, width, bottom=baseforsad, color='b')
p4 = plt.bar(ind, angry, width, bottom=baseforangry, color='r')

plt.xlabel('Time')
plt.ylabel('Customer')
plt.title('Customer Emotion Classified By Time')
plt.xticks(ind, timeline)
plt.yticks([0,5,10,15,20,25,30])
plt.legend((p1[0], p2[0], p3[0], p4[0]), ('Happy', 'Neutral','Sad','Angry'))

currenttime = fileName()
plt.savefig(currenttime)
plt.show()






