import pygame
import os

VOCAB_FILE_NAME = 'vocab.txt'

BLACK = (0, 0, 0)
WIDTH, HEIGHT = 800, 650
FPS = 60

TEXT_STYLE = 'Verdana'

SPC_BTW_BUTTONS = 15
BUTTON_HEIGHT = 50
BUTTON_LEN = 140
BUTTONS_STEP = 5
RADIUS = 12
LEFT_SPC_BUTTONS = 20
BUTTONS_Y = 10

BUTTON_COLORS = "#354B5E #475F77 #FFFFFF #D74B4B"
BUTTON_TXT_SZ = 23

KILLS_CNT_COLOR = "#000000"
KILLS_CNT_POS = (440, 500)
KILLS_CNT_TXT_SZ = 25

BAR_HEIGHT = 20
BAR_BORDER = 5
SPC_BTW_TXT_BARS = 10
BAR_COLORS = "#354B5E #475F77 #000000"
BAR_TXT_SZ = 30
BAR_SZ = (200, 40)

SLEEP_SPEED = 0.1
MOVE_SPEED = 8
TIME_SPEED = 1000

DLG_TXT_SZ = 15  # DIALOG
DLG_TXT_COLOR = "#000000"

game_folder = os.path.dirname(__file__)
img_folder = os.path.join(game_folder, 'img')

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Крутой пацан играть")
clock = pygame.time.Clock()

background_images = \
    {'day': pygame.image.load(
        os.path.join(img_folder, 'game_background_1.png')
    ).convert(),
     'night': pygame.image.load(
         os.path.join(img_folder, 'game_background_0.png')
     ).convert()}
player_img = pygame.image.load(os.path.join(img_folder, 'ben.png')).convert()

TIME, DAY_TIME = (0, 'day')


