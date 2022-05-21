from pet import Pet
from ui import *

pygame.init()
clock = pygame.time.Clock()
objects = []

player = Pet(pygame.font.SysFont(TEXT_STYLE, DLG_TXT_SZ), DLG_TXT_COLOR,
             VOCAB_FILE_NAME)
objects.append(player)

buttons_func = {"Накормить": player.feed, "Поспать": swap_day_time,
                "Играть": player.make_happy,
                "Говорить": player.talk, "Заново": player.kill}
for i, (text, func) in enumerate(buttons_func.items()):
    objects.append(
        Button(
            (LEFT_SPC_BUTTONS + i * (BUTTON_LEN + SPC_BTW_BUTTONS), BUTTONS_Y),
            (BUTTON_LEN, BUTTON_HEIGHT),
            BUTTON_COLORS, text, BUTTONS_STEP,
            pygame.font.SysFont(TEXT_STYLE, BUTTON_TXT_SZ),
            func))

names = {'energy': 'Энергия', 'happiness': 'Настроение', 'food': 'Сытость'}
bars = {}
for i, (attr, text) in enumerate(names.items()):
    i += 1
    bars[attr] = PropertyBar(
        (BAR_HEIGHT, HEIGHT - i * (BAR_SZ[1] + SPC_BTW_TXT_BARS)), BAR_SZ,
        BAR_COLORS,
        text,
        pygame.font.SysFont(TEXT_STYLE, BAR_TXT_SZ))

killCounter = CounterRect('Количество смертей', KILLS_CNT_POS, KILLS_CNT_COLOR,
                          pygame.font.SysFont(TEXT_STYLE, KILLS_CNT_TXT_SZ), player.show_kills)
objects.append(killCounter)

running = True
while running:
    clock.tick(FPS)

    if DAY_TIME == 'night':
        player.sleep()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    Background(background_images[DAY_TIME]).draw()

    for obj in objects:
        obj.draw()
        obj.update()

    for func, bar in bars.items():
        bar.draw()
        bar.update(player.values[func], player.maxVal[func])

    pygame.display.flip()