import PIL
import numpy as np
import math
from PIL import Image, ImageDraw

def user_func(image, x0, y0, x1, y1, fill, width):
    drawing =  ImageDraw.Draw(image)
    drawing.line(((x0, y0), (x1, y1)), fill=fill, width=width, joint=None)
    return image

#///////////////////////////////////////////////////

def check_coords(image, x0, y0, x1, y1):
    w, h = image.size
    if (x0 >= 0 and x1 >= 0 and y0 >= 0 and y1 >= 0) and (x1 > x0 and y1 > y0) and (x0 <= w and x1 <= w and y1 <= h and y0 <= h):
        return True
    

def set_black_white(image, x0, y0, x1, y1):
    if check_coords(image, x0, y0, x1, y1):
        cro = image.crop((x0, y0, x1, y1))
        cro = cro.convert('1')
        image.paste(cro, (x0, y0))

    return image

#///////////////////////////////////////////////////

def find_rect_and_recolor(image, old_color, new_color):
    w, h = image.size
    pixe = image.load()
    s_max = 0
    flag = False
    for x in range(w):
        for y in range(h):
            if pixe[x, y] == old_color:
                flag = True
                x1, y1 = x, y
                x2, y2 = x, y
                while (x2 < w) and (pixe[x2, y] == old_color):
                    x2 += 1
                while (y2 < h) and all((pixe[xz, y2] == old_color for xz in range(x1, x2))):
                    y2 += 1
                s_pi = (x2 - x1) * (y2 - y1)
                if (s_pi > s_max):
                    s_max = s_pi
                    x1max, y1max, x2max, y2max = x1, y1, x2, y2
    if flag:
        for xi in range(x1max,x2max):
            for yi in range(y1max, y2max):
                pixe[xi, yi] = new_color
    return image