from settings import *
from object import Object


def swap_day_time():
    global DAY_TIME
    DAY_TIME = 'day' * (DAY_TIME != 'day') + 'night' * (DAY_TIME != 'night')


class Background(Object):
    def __init__(self, image_file):
        self.image = image_file
        self.rect = self.image.get_rect()

    def draw(self):
        screen.blit(self.image, self.rect)

    def update(self):
        pass


class Button(Object):
    def __init__(self, pos, res, colors, input_text, elevation,
                 font, action=None,
                 *action_args):
        # сначала фон, потом основа, потом текст, потом сменный текст
        self.pressed = False
        self.elevation, self.dynamic = elevation, elevation
        self.pos_x, self.pos_y = pos
        self.font = font
        self.action = action
        self.action_args = action_args

        self.top_rect = pygame.Rect(pos, res)
        self.bottom_rect = pygame.Rect(pos, res)

        self.bottom_color, self.top_color, self.text_color, self.clicked_color \
            = colors.split(' ')

        self.static_top_color = self.top_color

        self.text = input_text
        self.text_surf = self.font.render(input_text, True, self.text_color)
        self.text_rect = self.text_surf.get_rect(center=self.top_rect.center)

    def change_text(self, new_text):
        self.text = new_text
        self.text_surf = self.font.render(new_text, True, self.text_color)
        self.text_rect = self.text_surf.get_rect(center=self.top_rect.center)

    def update(self):
        mouse_pos = pygame.mouse.get_pos()

        if self.top_rect.collidepoint(mouse_pos):

            self.top_color = self.clicked_color

            if pygame.mouse.get_pressed()[0]:
                self.dynamic = 0
                self.pressed = True
            else:
                self.dynamic = self.elevation
                if self.pressed:
                    self.pressed = not self.pressed
                    if self.action is not None:
                        self.action(*self.action_args)
                        if self.action == swap_day_time:
                            if self.text == 'Поспать':
                                self.change_text('Проснуться')
                            else:
                                self.change_text('Поспать')
        else:
            self.dynamic = self.elevation
            self.top_color = self.static_top_color

    def draw(self):
        self.top_rect.y = self.pos_y - self.dynamic
        self.text_rect.center = self.top_rect.center

        self.bottom_rect.midtop = self.top_rect.midtop
        self.bottom_rect.height = self.top_rect.height + self.dynamic

        pygame.draw.rect(screen, self.bottom_color, self.bottom_rect,
                         border_radius=RADIUS)
        pygame.draw.rect(screen, self.top_color, self.top_rect,
                         border_radius=RADIUS)
        screen.blit(self.text_surf, self.text_rect)


class PropertyBar:
    def __init__(self, pos, res, colors, input_text,
                 font):  # сначала фон, потом основа, потом текст
        self.progress = 0
        self.pos_x, self.pos_y = pos
        self.width, self.height = res

        self.top_rect = pygame.Rect(pos, res)
        self.bottom_color, self.top_color, self.text_color = colors.split(' ')

        self.text_surf = font.render(input_text, True, self.text_color)
        self.text_rect = \
            self.text_surf.get_rect(
                center=self.top_rect.center,
                left=self.top_rect.right + SPC_BTW_TXT_BARS)

    def draw(self):
        pygame.draw.rect(screen, self.top_color,
                         pygame.Rect(self.pos_x, self.pos_y,
                                     self.width * self.progress, self.height))
        pygame.draw.rect(screen, self.bottom_color,
                         pygame.Rect(self.pos_x, self.pos_y, self.width,
                                     self.height), BAR_BORDER)
        screen.blit(self.text_surf, self.text_rect)

    def update(self, progress, max_val=1):
        self.progress = progress / max_val


class CounterRect(Object):
    def __init__(self, name, pos, color, font, func):
        self.name = name
        self.text = name + ' : {}'
        self.font = font
        self.color = color
        self.text_surf = self.font.render(self.text, True, color)
        self.text_rect = pygame.Rect(pos, (0, 0))
        self.func = func

    def draw(self):
        screen.blit(self.text_surf, self.text_rect)

    def update(self):
        self.text_surf = self.font.render(self.text.format(self.func()), True,
                                          self.color)