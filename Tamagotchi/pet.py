from object import Object
import random
from settings import *
from ui import TIME, DAY_TIME


class Pet(Object):
    # IMPORTANT VARIABLES FOR CLASS
    keys = ['food', 'energy', 'happiness', 'age']
    reduce = dict(zip(keys, [3, 5, 7, -1]))
    warning = dict(zip(keys, [10, 10, 10, 700]))
    maxVal = dict(zip(keys, [100, 100, 100, 1000]))
    warnings_vocab = {
        'all is ok': 'Урааааа... Я такой довольный!! Хо-хо-хо, yeeees)',
        'hungry': 'Животик бо-бо, покормите едой пожалуйста(((',
        'old': "Старичок я уже, мне {} , помру скоро(",
        'bored': 'Мне грустно, потому что никто со мной не играет('}

    ###############################

    def __init__(self, font, text_color, file):
        with open(file, 'r', encoding='utf-8') as f:
            self.vocab = f.readlines()
        self.word = ''
        self.flagForTalks = True
        self.first_time = True
        self.kills_n = 0

        self.speedX, self.speedY = 0, 0

        self.font = font
        self.text_color = text_color

        self.values = {}
        for key in self.keys:
            self.values[key] = random.randrange(self.maxVal[key])

        self.image = player_img
        self.image.set_colorkey(BLACK)

        self.rect = self.image.get_rect()
        self.rect.center = (WIDTH / 2, HEIGHT / 2)

    def update(self):
        global TIME
        if pygame.time.get_ticks() // TIME_SPEED > TIME:
            TIME += 1
            index = random.randrange(0, len(self.keys) + 1)
            if index != len(self.keys):
                key = self.keys[index]
                self.values[key] -= self.reduce[key]
                self.balance_prop(key)

        self.speedX, self.speedY = 0, 0

        key_state = pygame.key.get_pressed()

        if key_state[pygame.K_a]:
            self.speedX = -MOVE_SPEED

        if key_state[pygame.K_d]:
            self.speedX = MOVE_SPEED

        if key_state[pygame.K_w]:
            self.speedY = -MOVE_SPEED

        if key_state[pygame.K_s]:
            self.speedY = MOVE_SPEED

        self.rect.x += self.speedX
        self.rect.y += self.speedY

    @property
    def mood(self):
        if self.values['food'] > self.warning['food'] and \
                self.values['happiness'] > self.warning['happiness']:
            return self.warnings_vocab['all is ok']

        if self.values['food'] <= self.warning['food']:
            return self.warnings_vocab['hungry']

        if self.values['age'] > self.warning['age']:
            return self.warnings_vocab['old'].format(self.values['age'] // 10)

        return self.warnings_vocab['bored']

    def talk(self):
        self.flagForTalks = not self.flagForTalks

    def draw(self):
        screen.blit(self.image,
                    self.rect)
        if self.flagForTalks:
            if self.first_time:
                self.word = self.vocab[random.randint(0, len(self.vocab) - 1)]
                self.first_time = not self.first_time
            self.show(self.word)
        else:
            self.first_time = True
            self.show(self.mood)

    def show(self, input_text):
        text_surf = self.font.render(input_text, True, self.text_color)
        text_rect = text_surf.get_rect(center=self.rect.center,
                                       left=self.rect.right + 10)

        screen.blit(text_surf, text_rect)

    def kill(self):
        self.kills_n += 1
        for key in self.keys:
            self.values[key] = random.randrange(self.maxVal[key])

    def balance_prop(self, prop):
        if self.values[prop] > self.maxVal[prop]:
            self.values[prop] = self.maxVal[prop]
            if prop == 'age':
                self.kill()
        if self.values[prop] < 0:
            self.kill()

    def feed(self):
        if not DAY_TIME == 'night':
            self.values['food'] += random.randrange(0,
                                                    self.maxVal['food'] // 10)
            self.balance_prop('food')

    def make_happy(self):
        if not DAY_TIME == 'night':
            self.values['happiness'] += random.randrange(0, self.maxVal[
                'happiness'] // 10)
            self.balance_prop('happiness')

    def sleep(self):
        self.values['energy'] += SLEEP_SPEED
        self.balance_prop('energy')

    def show_kills(self):
        return self.kills_n