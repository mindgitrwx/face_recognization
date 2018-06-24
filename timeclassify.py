
# example : print timeclassify('2015_6_20_3_40_30')
def timeclassify(string):
    newstr = string.split('_')
    hour = int(newstr[3])
    
    if 0 <= hour < 6:
        return 1
    elif 6 <= hour < 12:
        return 2
    elif 12 <= hour < 18:
        return 3
    elif 18 <= hour < 24:
        return 4
    else:
        return 0


#print(timeparser(str))
