import sys
import re
import math


class ParseException(Exception):
    def __init__(self, value):
        self._value = value

    def __str__(self):
        return str(self._value)


def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)


class point(object):
    """A point in a two dimensional space"""

    # print "In here XXXX"
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return '(' + pp(self.x) + ', ' + pp(self.y) + ')'


class line(object):
    """A line between two points"""

    def __init__(self, src, dst):
        self.src = src
        self.dst = dst

    def __repr__(self):
        return '[' + str(self.src) + '-->' + str(self.dst) + ']'


def make_edge(vertex1, vertex2):
    E_to_pass = []
    E_temp = (vertex1, vertex2)
    E_temp = sorted(E_temp, key=lambda y: y[0])
    E_temp = tuple(E_temp)
    # E_temp=set(E_temp)
    E_to_pass.append(E_temp)
    # print "The edges are:",E_to_pass
    return E_to_pass


def intersection(x11, y11, x12, y12, x21, y21, x22, y22):
    xnum = round(float(((x11 * y12 - y11 * x12) * (x21 - x22) - (x11 - x12) * (x21 * y22 - y21 * x22))),2)
    xden = round(float(((x11 - x12) * (y21 - y22) - (y11 - y12) * (x21 - x22))),2)
    ynum = round(float((x11 * y12 - y11 * x12) * (y21 - y22) - (y11 - y12) * (x21 * y22 - y21 * x22)),2)
    yden = round(float((x11 - x12) * (y21 - y22) - (y11 - y12) * (x21 - x22)),2)
    # V_temp=set()
    V_temp = []
    if (xden != 0 and yden != 0):
        xcoor = round(float(xnum / xden),2)
        #xcoor="{0:.2f}".format(xcoor)
        ycoor = round(float(ynum / yden),2)
        #ycoor="{0:.2f}".format(ycoor)
        if ((x11 <= xcoor <= x12 or x11 >= xcoor >= x12) and (y11 <= ycoor <= y12 or y11 >= ycoor >= y12) and (
                x21 <= xcoor <= x22 or x21 >= xcoor >= x22) and (y21 <= ycoor <= y22 or y21 >= ycoor >= y22)):
            v1 = (round(float(x11),2),round(float(y11),2))
            v2 = (round(float(x12),2),round(float(y12),2))
            v3 = (round(float(x21),2),round(float(y21),2))
            v4 = (round(float(x22),2),round(float(y22),2))
            v5 = (xcoor, ycoor)
            l1 = [v1, v2]
            l2 = [v3, v4]
            # l=[l1,l2]
            V_temp.append(v1)
            V_temp.append(v2)
            V_temp.append(v3)
            V_temp.append(v4)
            V_temp.append(v5)
            global counter
            inter_line[v5, counter] = l1, l2
            # var=str(v5)+str(flag)
            # inter_line[var]=l1,l2
            counter += 1
            return V_temp
        else:
            # print "Error :There is no intersection"
            return V_temp
    else:
        # print "There is no intersection:"
        return V_temp
    return V_temp


def create_graph():
    points_in_street = [[i for i in street_coordinates[x]] for x in street_coordinates.keys()]
    V_temp1 = set()
    E_temp1 = set()
    # V_temp1=[]
    for r in range(len(points_in_street)):
        for s in range(len(points_in_street[r])):
            if (s != len(points_in_street[r]) - 1):
                p1 = re.findall(r'[-+]?\d+', points_in_street[r][s])
                x11 = int(p1[0])
                y11 = int(p1[1])
                p2 = re.findall(r'[-+]?\d+', points_in_street[r][s + 1])
                x12 = int(p2[0])
                y12 = int(p2[1])
                l = r + 1
                # m=s+1
                if (l != len(points_in_street)):
                    for i in range(l, len(points_in_street)):
                        for j in range(len(points_in_street[i])):
                            if (j != len(points_in_street[i]) - 1):
                                p3 = re.findall(r'[-+]?\d+', points_in_street[i][j])
                                x21 = int(p3[0])
                                y21 = int(p3[1])
                                p4 = re.findall(r'[-+]?\d+', points_in_street[i][j + 1])
                                x22 = int(p4[0])
                                y22 = int(p4[1])
                                V_temp = intersection(x11, y11, x12, y12, x21, y21, x22, y22)
                                if len(V_temp1) == 0:
                                    V_temp1 = set(V_temp).copy()
                                else:
                                    V_temp1 = V_temp1.union(set(V_temp))
    ListofValues = []
    #print "Inter_line:",inter_line
    for i in inter_line:
        for j in inter_line[i]:
            ListofValues.append(j)
    listOfKeys = []
    for i in ListofValues:
        listOfKeys = [key for key, value in inter_line.items() if i in value]
        tupleOfKeys = tuple(listOfKeys)
        line_inter[tuple(i)] = tupleOfKeys
    for i in line_inter:
        if (len(line_inter[i]) == 1):
            p1 = i[0]
            p2 = line_inter[i][0]
            p3 = i[1]
            E_temp = make_edge(p1, p2)
            if len(E_temp1) == 0:
                E_temp1 = set(E_temp).copy()
            E_temp1 = E_temp1.union(set(E_temp))
            E_temp = make_edge(p2, p3)
            if len(E_temp1) == 0:
                E_temp1 = set(E_temp).copy()
            else:
                E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                E_temp1 = E_temp1.union(set(E_temp))
        if (len(line_inter[i]) == 2):
            p1 = i[0]
            #print "p1", p1
            p2 = line_inter[i]
            #print "p2",p2
            p21 = p2[0][0]
            #print "p21",p21
            p22 = p2[1][0]
            #print "p22",p22
            p3 = i[1]
            #print "p3",p3
            distance1 = math.sqrt((p1[0] - p21[0]) ** 2 + (p1[1] - p21[1]) ** 2)
            distance2 = math.sqrt((p1[0] - p22[0]) ** 2 + (p1[1] - p22[1]) ** 2)
            distance3 = math.sqrt((p3[0] - p21[0]) ** 2 + (p3[1] - p21[1]) ** 2)
            distance4 = math.sqrt((p3[0] - p22[0]) ** 2 + (p3[1] - p22[1]) ** 2)
            if (distance1 < distance2):
                E_temp = make_edge(p1, p21)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            else:
                E_temp = make_edge(p1, p22)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            if (distance3 < distance4):
                E_temp = make_edge(p3, p21)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            else:
                E_temp = make_edge(p3, p22)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            if (p21 != p22):
                #print "In not equal"
                E_temp=make_edge(p21,p22)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
                '''p1 = i[0]
                #print "In p1",p1
                p2 = line_inter[i][0]
                #print "In p2",p2
                p3 = i[1]
                #print "In p3",p3
                E_temp = make_edge(p1, p2)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp1 = E_temp1.union(set(E_temp))

                E_temp = make_edge(p2, p3)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp1 = E_temp1.union(set(E_temp))'''
        if (len(line_inter[i]) == 3):
            p1 = i[0]
            p2 = line_inter[i]
            p2i=line_inter[i][0]
            p21 = p2[0][0]
            p22 = p2[1][0]
            p23=p2[2][0]
            p3 = i[1]
            distance1 = math.sqrt((p1[0] - p21[0]) ** 2 + (p1[1] - p21[1]) ** 2)
            distance2 = math.sqrt((p1[0] - p22[0]) ** 2 + (p1[1] - p22[1]) ** 2)
            distance11=math.sqrt((p1[0] - p23[0]) ** 2 + (p1[1] - p23[1]) ** 2)
            distance3 = math.sqrt((p3[0] - p21[0]) ** 2 + (p3[1] - p21[1]) ** 2)
            distance4 = math.sqrt((p3[0] - p22[0]) ** 2 + (p3[1] - p22[1]) ** 2)
            distance31=math.sqrt((p3[0] - p23[0]) ** 2 + (p3[1] - p23[1]) ** 2)
            if (distance1 < distance2 and distance1<distance11):
                E_temp = make_edge(p1, p21)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            elif (distance2<distance1 and distance2<distance11):
                E_temp = make_edge(p1, p22)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            else:
                E_temp = make_edge(p1, p23)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            if (distance3 < distance4 and distance3< distance31):
                E_temp = make_edge(p3, p21)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            elif(distance4 < distance3 and distance4< distance31):
                E_temp = make_edge(p3, p22)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            else:
                E_temp = make_edge(p3, p23)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
            '''distance1 = math.sqrt((p21[0] - p22[0]) ** 2 + (p21[1] - p22[1]) ** 2)
            distance2 = math.sqrt((p22[0] - p23[0]) ** 2 + (p22[1] - p23[1]) ** 2)
            distance3 = math.sqrt((p23[0] - p21[0]) ** 2 + (p23[1] - p21[1]) ** 2)'''
            if((p22[0]<=p21[0]<=p23[0] or p22[0]>=p21[0]>=p23[0]) and (p22[1]<=p21[1]<=p23[1] or p22[1]>=p21[1]>=p23[1])):
                E_temp=make_edge(p22,p21)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))
                E_temp = make_edge(p21, p23)
                if len(E_temp1) == 0:
                    E_temp1 = set(E_temp).copy()
                else:
                    E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                    E_temp1 = E_temp1.union(set(E_temp))

            if ((p21[0] <= p22[0] <= p23[0] or p21[0]>= p22[0] >= p23[0]) and (p21[1]<= p22[1]<= p23[1] or p21[1]>=p22[1]>=p23[1])):
                if(p21!=p22):
                    E_temp = make_edge(p21, p22)
                    if len(E_temp1) == 0:
                        E_temp1 = set(E_temp).copy()
                    else:
                        E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                        E_temp1 = E_temp1.union(set(E_temp))
                if(p22!=p23):
                    E_temp = make_edge(p22, p23)
                    if len(E_temp1) == 0:
                        E_temp1 = set(E_temp).copy()
                    else:
                        E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                        E_temp1 = E_temp1.union(set(E_temp))
            if ((p21[0]<= p23[0] <= p22[0] or p21[0] >= p23[0] >= p22[0]) and (p21[1]<= p23[1] <= p22[1] or p21[1] >= p23[1] >=p22[1])):
                if(p21!=p23):
                    E_temp = make_edge(p21, p23)
                    if len(E_temp1) == 0:
                        E_temp1 = set(E_temp).copy()
                    else:
                        E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                        E_temp1 = E_temp1.union(set(E_temp))
                if(p23 != p22):
                    E_temp = make_edge(p23, p22)
                    if len(E_temp1) == 0:
                        E_temp1 = set(E_temp).copy()
                    else:
                        E_temp = sorted(tuple(E_temp), key=lambda y: y[0])
                        E_temp1 = E_temp1.union(set(E_temp))
    #print "E_temp1:",E_temp1
    V_temp1 = list(V_temp1)
    # Printing Vertices:
    vertices="V "+str(len(V_temp1))
    sys.stdout.write(vertices)
    sys.stdout.flush()
    sys.stdout.write("\n")
    sys.stdout.flush()
    '''for i in range(len(V_temp1)):
            print i, ":  ", V_temp1[i]
    sys.stdout.write("}")'''
    sys.stdout.write("E {")
    sys.stdout.flush()
    E_temp2 = list(E_temp1)
    #print "E_temp2",E_temp2
    for i in range(len(E_temp2)):
        if i != (len(E_temp2) - 1):
            if (type(E_temp2[i][1][0]) is tuple) == True:
                if(E_temp2[i][0] in V_temp1 and E_temp2[i][1][0] in V_temp1):
                    if(E_temp2[i][0] != E_temp2[i][1][0]):
                        k1 = V_temp1.index(E_temp2[i][0])
                        k2 = V_temp1.index(E_temp2[i][1][0])
                        if(i==len(E_temp2)-1):
                            string = "<" + str(k1) + "," + str(k2) + ">"
                        else:
                            string = "<" + str(k1) + "," + str(k2) + ">,"
                        sys.stdout.write(string)
                        sys.stdout.flush()
            else:
                if(E_temp2[i][0] in V_temp1 and E_temp2[i][1] in V_temp1):
                    if(E_temp2[i][0] != E_temp2[i][1]):
                        k1 = V_temp1.index(E_temp2[i][0])
                        k2 = V_temp1.index(E_temp2[i][1])
                        if (i == len(E_temp2)-1):
                            string = "<" + str(k1) + "," + str(k2) + ">"
                        else:
                            string = "<" + str(k1) + "," + str(k2) + ">,"
                        sys.stdout.write(string)
                        sys.stdout.flush()

        else:
            if (type(E_temp2[i][1][0]) is tuple) == True:
                if(E_temp2[i][0] in V_temp1 and E_temp2[i][1][0] in V_temp1):
                    if(E_temp2[i][0] != E_temp2[i][1][0]):
                        k1 = V_temp1.index(E_temp2[i][0])
                        k2 = V_temp1.index(E_temp2[i][1][0])
                        if (i == len(E_temp2)-1):
                            string = "<" + str(k1) + "," + str(k2) + ">"
                        else:
                            string = "<" +str(k1)+ "," +str(k2)+ ">,"
                        sys.stdout.write(string)
                        sys.stdout.flush()
            else:
                if(E_temp2[i][0] in V_temp1 and E_temp2[i][1] in V_temp1):
                    if(E_temp2[i][0] != E_temp2[i][1]):
                        k1 = V_temp1.index(E_temp2[i][0])
                        k2 = V_temp1.index(E_temp2[i][1])
                        if (i == len(E_temp2)-1):
                            string = "<" + str(k1) + "," + str(k2) + ">"
                        else:
                            string="<"+str(k1)+","+str(k2)+">,"
                        sys.stdout.write(string)
                        sys.stdout.flush()                    
    sys.stdout.write("}\n")
    sys.stdout.flush()


def add_street(coordinates_add, pattern_extract_streetname):
    if pattern_extract_streetname in streets:
        raise ParseException("street currently exists.")
    street_coordinates.setdefault(pattern_extract_streetname[0].upper(), [])
    for coordinate in coordinates_add:
        street_coordinates[pattern_extract_streetname[0].upper()].append(coordinate)
    #print "In add: pattern_extract_streetname:",pattern_extract_streetname
    #print "In add: street_coordinates:", street_coordinates
    streets.append(pattern_extract_streetname[0].upper())
def change_street(coordinates_remove, pattern_extract_streetname):
    if (street_coordinates.pop(pattern_extract_streetname[0].upper(), 'Not present') == 'Not present'):
        raise ParseException("There is no such street present which can be changed")
    else:
        street_coordinates.setdefault(pattern_extract_streetname[0].upper(), [])
        for coordinate in coordinates_remove:
            street_coordinates[pattern_extract_streetname[0].upper()].append(coordinate)
    #print "In change:street_coordinates:",street_coordinates
    # streets.append(pattern_extract_streetname)


def remove_street(pattern_extract_streetname):
    if (street_coordinates.pop(pattern_extract_streetname[0].upper(), 'Not present') == 'Not present'):
        raise ParseException("There is no such street present which can be removed")
    #print "In remove: pattern_extract_streetname:",pattern_extract_streetname
    #print "Before remove:streets:",streets
    streets.remove(pattern_extract_streetname[0].upper())
    #print "After remove:streets:",streets

# Check the input if its correct
def check(text):
    cond_digit = r'^[-+]?[0-9]+$'
    # change street conditions
    cond_change = r'\b^c[\s+]'
    pattern_change = re.findall(cond_change, text, re.MULTILINE)
    # Remove street conditions
    cond_remove = r'\b^r[\s+]'
    pattern_remove = re.findall(cond_remove, text, re.MULTILINE)
    # Add street conditions
    cond_add = r'\b^a\s+'
    pattern_add = re.findall(cond_add, text, re.MULTILINE)
    # Street conditions
    cond_street = r'\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"\s'
    pattern_street = re.findall(cond_street, text, re.MULTILINE)
    # Street condition for add
    cond_street_add = r'\b^a[\s+]\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"\s'
    pattern_street_add = re.findall(cond_street_add, text, re.MULTILINE)
    # Street condition for remove
    cond_street_remove = r'\b^r[\s+]\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"\s*'
    pattern_street_remove = re.findall(cond_street_remove, text, re.MULTILINE)
    # Street condition for change
    cond_street_change = r'\b^c[\s+]\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"\s'
    pattern_street_change = re.findall(cond_street_change, text, re.MULTILINE)
    # Co-ordinate conditions
    cond_coordinates = r'\([-+]?\d+\,[-+]?\d+\)[\s]*'
    pattern_coordinates = re.findall(cond_coordinates, text, re.MULTILINE)
    # Co-ordinates conditions for add
    cond_coordinates_add = r'\b^a[\s+]\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"\s\([-+]?\d+\,[-+]?\d+\)[\s]*'
    pattern_coordinates_add = re.findall(cond_coordinates_add, text, re.MULTILINE)
    # Co-ordinates conditions for remove
    cond_coordinates_remove = r'\b^r[\s+]\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"\s\([-+]?\d+\,[-+]?\d+\)[\s]*'
    pattern_coordinates_remove = re.findall(cond_coordinates_remove, text, re.MULTILINE)
    # Co-ordinates conditions for change
    cond_coordinates_change = r'\b^c[\s+]\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"\s\([-+]?\d+\,[-+]?\d+\)[\s]*'
    pattern_coordinates_change = re.findall(cond_coordinates_change, text, re.MULTILINE)
    # Condition for creating graph
    pattern_create_graph = re.findall(r'^\b^g\s*', text, re.MULTILINE)
    pattern_check_create_graph = re.findall(r'\b^g\s*\w+\s*\d+\s*\d+\s*\w+\s*', text, re.MULTILINE)
    if (pattern_add or pattern_change or pattern_remove):
        if (pattern_street):
            if (pattern_coordinates):
                if (('a ' in pattern_add) or ('a  'in pattern_add) or ('a   ' in pattern_add)):
                    if (pattern_street_add):
                        if pattern_coordinates_add:
                            pattern_extract_coordinates = re.findall(r'\([-+]?\d+\,[-+]?\d+\)',str(pattern_coordinates))
                            coordinates_add = []
                            for coordinate in pattern_extract_coordinates:
                                # print "pattern_street_add:", pattern_street_add
                                coordinates_add.append(coordinate)
                                coordinates.append(coordinate)
                            pattern_extract_streetname = re.findall(r'\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"', str(pattern_street_add))
                            #print "Pattern_extract_streetname:",pattern_extract_streetname
                            add_street(coordinates_add, pattern_extract_streetname)
                        else:
                            raise ParseException("Incorrect format for coordinates")
                    else:
                        raise ParseException("Incorrect format for adding street")
                elif ('c ' in pattern_change or 'c  ' in pattern_change):
                    if (pattern_street_change):
                        if (pattern_coordinates_change):
                            pattern_extract_coordinates = re.findall(r'\([-+]?\d+\,[-+]?\d+\)',str(pattern_coordinates))
                            for coordinate in pattern_extract_coordinates:
                                # print "pattern_street_add:", pattern_street_add
                                coordinates_remove.append(coordinate)
                            pattern_extract_streetname = re.findall(r'\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"', str(pattern_street_change))
                            global inter_line
                            inter_line={}
                            change_street(coordinates_remove, pattern_extract_streetname)
                        else:
                            raise ParseException("Incorrect format for coordinates")
                    else:
                        raise ParseException("Incorrect format for changing street")
                else:
                    raise ParseException("Incorrect input format")
            elif ('r ' in pattern_remove):
                if pattern_street_remove:
                    if (pattern_coordinates_remove):
                        raise ParseException("Remove cannot have coordinates")
                    else:
                        # pattern_extract_coordinates = re.findall(r'\([-+]?\d+\,[-+]?\d+\)',str(pattern_coordinates))
                        pattern_extract_streetname = re.findall(r'\"\s*[a-zA-Z]+\s*[a-zA-Z]*\s*[a-zA-Z]*\s*[a-zA-Z]*\s*\"', str(pattern_street_remove))
                        inter_line={}
                        remove_street(pattern_extract_streetname)
                else:
                    raise ParseException("Incorrect format for removing street")
            else:
                raise ParseException("Pattern coordinates are in incorrect format")

        else:
            raise ParseException("Street name incorrect")
    elif (pattern_create_graph):
        if (pattern_check_create_graph):
            raise ParseException("g is in incorrect format")
        else:
            create_graph()

    else:
        raise ParseException("Incorrect input format")


def run(input, out, err):
    global pattern_extract_streetname
    pattern_extract_streetname = []
    global counter
    counter = 1
    global inter_line
    inter_line = {}
    global v1
    global v2
    global v3
    global v4
    global v5
    global x
    x = 'x'
    global E
    E = set()
    global Edges
    Edges = set()
    global line
    line = 'l'
    global points_in_street
    points_in_street = []
    global store_point
    store_point = []
    global street_coordinates
    street_coordinates = {}
    global streets
    streets = []
    global coordinates
    coordinates = []
    global coordinates_remove
    coordinates_remove = []
    global V
    V = set()
    #global V_new
    #V_new = []

    global line_inter
    line_inter = {}
    while True:
        line = sys.stdin.readline()
        if line == '':
            break
        try:
            check(line)
            # lines.append(line)
        except ParseException as ex:
            sys.stderr.write("Error: {0}\n".format(ex))
            sys.stdout.flush()
            sys.stderr.flush()
    #print 'Finished reading input'
    sys.exit(0)


def main():
    run(sys.stdin, sys.stdout, sys.stderr)


if __name__ == '__main__':
    main()
