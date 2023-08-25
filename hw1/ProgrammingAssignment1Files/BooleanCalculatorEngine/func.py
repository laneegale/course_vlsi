import os
import glob
import copy

def read_file(fp):
    with open(fp, "r") as f:
        content = f.read()

    return content

def read_raw(content: str):
    content_split = content.split("\n")

    if len(content_split) < 2:
        raise Exception("Data read exception")

    num_var = int(content_split[0])
    # num_clauses = content_split[1]

    # start evalute
    
    clauses = []

    for c in content_split[2:]:
        if c.strip() == "":
            continue
        
        var_list = [0] * num_var
        var_list = eval_clause(c, var_list)
        clauses.append(var_list)

    return clauses, num_var

def eval_clause(clause, rtn):
    """
    args:
    clause: str
            e.g. "3 2 3 4"
    
    rtn:
    out:    list
            [0, 1, 1, 1]
    """

    spt = clause.strip().split(" ")

    if len(spt) < 2:
        return rtn

    for i in spt[1:]:
        i = int(i)
        rtn[abs(i)-1] = int(i / abs(i))

    return rtn

def urp_complement(cubeList, num_var):
    if is_simple(cubeList):
        return complement(cubeList, num_var)
    else:
        x = find_most_binate(cubeList, num_var)

        l = urp_complement(positive_cofac(cubeList, x), num_var)
        r = urp_complement(negative_cofac(cubeList, x), num_var)

        l = bin_and(x, l, 1)
        r = bin_and(x, r, -1)

        return opt_or(l, r)
    
def is_simple(cubeList):
    # Empty cubelist
    if len(cubeList) == 0:
        return True
    
    # Contains all dont care
    for i in cubeList:
        if len(set(i)) == 1 and i[0] == 0:
            return True
    
    # Contains only one cube
    if len(cubeList) == 1:
        return True
    
    return False

def complement(cubeList, num_var):
    if len(cubeList) == 0:
        return [[0] * num_var]
    
    for i in cubeList:
            if len(set(i)) == 1 and i[0] == 0:
                return []
    
    if len(cubeList) == 1:

        rtn = []
        for i in range(num_var):
            if cubeList[0][i] == 0:
                continue
            
            new_clause = [0] * num_var
            new_clause[i] = int(-cubeList[0][i] / abs(cubeList[0][i]))

            rtn.append(new_clause)
        
        return rtn

def find_most_binate(cubeList, num_var):
    """
    rtn:
    idx: variable id. start from 0
    """

    track_table = [[0, 0, num_var-i, i] for i in range(num_var)]

    import numpy as np

    temp = np.array(cubeList).T
    if len(temp) == 0:
        return None

    is_binate = []
    for i in range(num_var):
        if 1 in temp[i] and -1 in temp[i]:
            is_binate.append(i)
        appear_amount = sum([abs(j) for j in temp[i]])
        binate_score = sum([1 for j in temp[i] if j == 1]) - \
                        sum([1 for j in temp[i] if j ==-1])
        binate_score = abs(binate_score)

        track_table[i][0] = appear_amount
        track_table[i][1] = num_var - binate_score

    from functools import cmp_to_key

    def compare_arr(x, y):
        if x[0] > y[0]:
            return 1
        elif x[0] == y[0]:
            if x[1] > y[1]:
                return 1
            elif x[1] == y[1]:
                if x[2] >= y[2]:
                    return 1
                else:
                    return -1
        return -1
    
    track_table.sort(key=cmp_to_key(compare_arr))

    track_table_binate_only = [i for i in track_table if i[-1] in is_binate]

    if len(track_table_binate_only) == 0:
        return track_table[-1][-1]
    
    return track_table_binate_only[-1][-1]

def positive_cofac(cubeList, var_id):
    pop_list = []
    rtn = copy.deepcopy(cubeList)

    for i in range(len(cubeList)):
        if rtn[i][var_id] == 1:
            rtn[i][var_id] = 0
        elif rtn[i][var_id] == -1:
            pop_list.append(i)

    for i in pop_list[::-1]:
        rtn.pop(i)
    
    return rtn

def negative_cofac(cubeList, var_id):
    pop_list = []
    rtn = copy.deepcopy(cubeList)

    for i in range(len(cubeList)):
        if rtn[i][var_id] == -1:
            rtn[i][var_id] = 0
        elif rtn[i][var_id] == 1:
            pop_list.append(i)

    for i in pop_list[::-1]:
        rtn.pop(i)
    
    return rtn

def bin_and(var_id, cubeList, val):
    rtn = copy.deepcopy(cubeList)
    poplist = []

    for i in range(len(cubeList)):
        temp = rtn[i]

        if temp[var_id] == 0:
            temp[var_id] = val
        elif temp[var_id] != val:
            poplist.append(i)
        
    for i in poplist[::-1]:
        rtn.pop(i)
    
    return rtn

def opt_or(l_cubeList, r_cubeList):
    rtn = copy.deepcopy(l_cubeList)

    for i in r_cubeList:
        if i not in l_cubeList:
            rtn.append(i)
    
    return rtn

def transform_cube_to_str(cube):
    temp = []
    for i in range(len(cube)):
        if cube[i] != 0:
            temp.append(cube[i] * (i+1))

    out = str(len(temp))

    for i in temp:
        out += " " + str(i)

    return out


def write_result(result, filename):
    with open(filename, "w") as f:
        f.write(result[0]+"\n")
        f.write(result[1]+"\n")
        for i in result[2:]:
            f.write(transform_cube_to_str(i)+"\n")
    
    return True

def pipeline(fp):

    content = read_file(fp)
    cubeList, num_var = read_raw(content)

    result = urp_complement(cubeList, num_var)

    result.insert(0, str(len(result)))
    result.insert(0, str(num_var))
    
    return result