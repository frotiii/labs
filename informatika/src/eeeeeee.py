import numpy as np


def check_crossroad(robot, point1, point2, point3, point4):
    return (point1[0] <= robot[0] <= point3[0]) and (point2[1] <= robot[1] <= point4[1])


def check_collision(coefficients):
    stroki = coefficients.shape[0]
    result = []
    for i in range(stroki):
        for j in range(stroki):
            a1, b1, c1 = list(map(int, coefficients[i]))
            a2, b2, c2 = list(map(int, coefficients[j]))
            arr_a_b = np.array([[a1, b1], [a2, b2]])
            arr_c = np.array([-c1, -c2])
            try:
                res = np.linalg.solve(arr_a_b, arr_c)
                result.append((i, j))
            except:
                ...
    return result



def check_path(points_list):
    result = 0
    for i in range(0, len(points_list)-1):
        dlina = ((points_list[i][0]-points_list[i+1][0])**2 + (points_list[i][1]-points_list[i+1][1])**2)**(0.5)
        result += dlina
    return round(result, 2)



    
    


        
