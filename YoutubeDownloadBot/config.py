import sqlite3

from aiogram import Bot, Dispatcher

TOKEN = '5379183801:AAGc1Li3ATmejd0gFAwi0NkEUbpaXo8zZHo'

START_TEXT = 'Привет, {}! Этот Бот поможет тебе скачивать видео или аудио из Youtube. ' \
             'Это работает автоматически, ничего не нужно делать. ' \
             'Просто набери в нашем чате @Music123227bot + запрос поиска в Youtube, ' \
             'а затем нажми на видео, из которого нужно что-то скачать. ' \
             'Помимо этого в чатах без меня ты можешь использовать те же самые действия для удобного поиска видео, ' \
             'чтобы отправить другу. Удачи!'
ERR_TEXT = 'Я не понимаю. Напиши /start, пришли ссылку на видео или @Music123227bot + запрос поиска в Youtube.'
WAIT_TEXT = 'Подожди немного, файл загружается...\nОбычно это занимает не более 3-ёх минут.'

FILE_SIZE_ERRTXT = 'Файл слишком большой(Больше 50Мб)({})'
FILES_UPLOADED = 'Файлы загружены!'

SITE = 'https://www.youtube.com'
START_QUERY = 'OG Buda'

MAX_SIZE = 52428800

connection = sqlite3.connect('bot_queries.db')
bot = Bot(TOKEN)
dp = Dispatcher(bot)
create_table_users = '''CREATE TABLE messages (
                                message_id INTEGER PRIMARY KEY,
                                date datetime,
                                user_id INTEGER NOT NULL,
                                user_name TEXT NOT NULL,
                                chat_id INTEGER NOT NULL,
                                message_text TEXT,
                                video TEXT,
                                audio TEXT);'''
