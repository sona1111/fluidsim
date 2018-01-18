import pygame
from pygame.locals import *
import time
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *


WINDOW_WIDTH = 1300
WINDOW_HEIGHT = 1300

def glut_print( x,  y,  text):


    # glMatrixMode(GL_PROJECTION)
    # glPushMatrix()
    # glLoadIdentity()
    # gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT)
    # glMatrixMode(GL_MODELVIEW)

    glColor3f(1,1,1)
    glRasterPos2f(x,y)
    for ch in text :
        glutBitmapCharacter(OpenGL.GLUT.fonts.GLUT_BITMAP_9_BY_15 , ctypes.c_int( ord(ch) ) )

    # glMatrixMode(GL_MODELVIEW)
    # glPopMatrix()
    # glMatrixMode(GL_PROJECTION)
    # glPopMatrix()


GRID_SIZE_X = 7.0
GRID_SIZE_Y = 7.0
GRID_SPACING = 1.0 / GRID_SIZE_Y
def Grid():



    for x in range(int(GRID_SIZE_X)):
        glBegin(GL_LINES)
        glVertex3fv((GRID_SPACING * x, GRID_SPACING*(GRID_SIZE_Y), 0))
        glVertex3fv((GRID_SPACING * x, 0, 0))
        glEnd()
    for y in range(int(GRID_SIZE_Y)):
        glBegin(GL_LINES)
        glVertex3fv((0, GRID_SPACING * y, 0))
        glVertex3fv((GRID_SPACING * (GRID_SIZE_X), GRID_SPACING * y, 0))
        glEnd()

    for x in range(int(GRID_SIZE_X)):
        for y in range(int(GRID_SIZE_Y)):
            glPushMatrix()
            # begin in the center
            glTranslatef((x * GRID_SPACING) + (GRID_SPACING / 2), (y * GRID_SPACING) + (GRID_SPACING / 2), 0)
            # draw vertical arrow from the bottom up
            glBegin(GL_LINE_STRIP)
            glVertex3fv((0, -(GRID_SPACING / 2), 0))
            glVertex3fv((0, -(GRID_SPACING / 2)+(GRID_SPACING / 8), 0))
            glVertex3fv(((GRID_SPACING / 16), -(GRID_SPACING / 2) + (GRID_SPACING / 16), 0))
            glVertex3fv((-(GRID_SPACING / 16), -(GRID_SPACING / 2) + (GRID_SPACING / 16), 0))
            glVertex3fv((0, -(GRID_SPACING / 2) + (GRID_SPACING / 8), 0))
            glEnd()
            # draw horizontal arrow from the left to right
            glBegin(GL_LINE_STRIP)
            glVertex3fv((-(GRID_SPACING / 2), 0, 0))
            glVertex3fv((-(GRID_SPACING / 2)+(GRID_SPACING / 8),0, 0))
            glVertex3fv((-(GRID_SPACING / 2) + (GRID_SPACING / 16), (GRID_SPACING / 16), 0))
            glVertex3fv((-(GRID_SPACING / 2) + (GRID_SPACING / 16), -(GRID_SPACING / 16), 0))
            glVertex3fv((-(GRID_SPACING / 2) + (GRID_SPACING / 8), 0, 0))
            glEnd()

            glPopMatrix()

    # glRotatef(angles[grid[2]], 0, 0, 1)

def Values(frame):
    str_loc = 0
    for x in range(int(GRID_SIZE_X)):
        for y in range(int(GRID_SIZE_Y)):
            glPushMatrix()
            # begin in the center
            glTranslatef((x * GRID_SPACING) + (GRID_SPACING / 2), (y * GRID_SPACING) + (GRID_SPACING / 2), 0)
            # draw vertical flow value from bottom
            glut_print(-(GRID_SPACING / 2) + (GRID_SPACING / 16), -GRID_SPACING / 3, frame[str_loc][1])
            # draw horizontal flow value from left
            glut_print(-(GRID_SPACING / 2) + (GRID_SPACING / 16), -GRID_SPACING/6, frame[str_loc][0])
            # draw pressure value
            glut_print(0, 0, frame[str_loc][2])
            # draw coordinates, content
            glut_print((GRID_SPACING * 0.2), (GRID_SPACING * 0.4), "%d,%d,%s" % (x, y, frame[str_loc][3]))


            glPopMatrix()
            str_loc += 1



yLength = 0
xLength = 0
numFrames = 0


def read_frames():
    """
    file format:
    -each line is one frame
    -each frame contains a number of call data separated by ';'
    -each cell data is a comma separated list of vx,vy,p , where
        vx is the velocity out of the left face, pointing right and
        vy is the velocity out of the top face, pointing down
    """
    global xLength, yLength, numFrames
    frames = []
    with open('/home/paul/dev/fluidsim/py/frames.dat', 'r') as f:
        for line in f:
            frame = []
            for cell in line.split(';'):
                d = cell.split(',')
                frame.append((str(d[0]), str(d[1]), str(d[2]), str(d[3]),))
            frames.append(frame)
    xLength = len(frames[0])
    yLength = len(frames[0])
    numFrames = len(frames)
    return frames



doneFlag = False
currentFrame = 0
winID = 0

def display():


    # global doneFlag
    #
    # if doneFlag:
    #     return False

    frames = read_frames()

    #glTranslate(0, 0, -1.00001)

    # while(True):

    glClear(GL_COLOR_BUFFER_BIT)
    Grid()
    Values(frames[currentFrame])
    glFlush()



    #
    #
    #
    #
    # for frame in frames:
    #
    #     glClear(GL_COLOR_BUFFER_BIT)
    #     Grid()
    #     Values(frame)
    #
    #     glFlush()
    #
    #     time.sleep(0.1)
    #
    # doneFlag = True

def keyboard(key, x, y):
    global currentFrame, winID, numFrames

    if key == b'z':
        if currentFrame > 0:
            currentFrame -= 1
    if key == b'x':
        if currentFrame < numFrames-1:
            currentFrame += 1
    if key == b'c':
        currentFrame = 0

    if key == b'q':
        glutDestroyWindow(winID)
        exit(0)
        return False

    glutPostRedisplay()

def main2():
    global xLength, yLength, winID

    def init():
        # select clearing color
        glClearColor(0.0, 0.0, 0.0, 0.0)

        # initialize viewing values
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0)

    #  Declare initial window size, position, and display mode
    #  (single buffer and RGBA).  Open window with "hello"
    #  in its title bar.  Call initialization routines.
    #  Register callback function to display graphics.
    #  Enter main loop and process events.



    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT)
    glutInitWindowPosition(0, 0)
    winID = glutCreateWindow("Fluid Simulator")


    init()

    #glTranslatef(0.7,0.7,1)

    glutKeyboardFunc(keyboard)
    glutDisplayFunc(display)


    glutMainLoop()










def main():
    global xLength, yLength

    pygame.init()
    display = (800,800)
    pygame.display.set_mode(display, DOUBLEBUF|OPENGL)

    glutInit()

    gluPerspective(45, (display[0]/display[1]), 0.1, 50.0)

    glTranslatef(-GRID_SIZE_X*GRID_SPACING/2,-GRID_SIZE_Y*GRID_SPACING/2, -10)

    frames = read_frames()

    for frame in frames:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                quit()

        #glRotatef(1, 3, 1, 1)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)

        glut_print(1, 2, "test")

        #Grid()
        print(frame)

        #Arrows(frame)
        pygame.display.flip()
        pygame.time.wait(1000)


main2()