import hashlib

from aiogram import types, executor

from config import *
from help_funcs import searcher, send
from init_table import create_table
from logger import save_logs

create_table()


@dp.inline_handler()
async def inline_handler(query: types.InlineQuery):
    links = searcher(query.query or START_QUERY)
    articles = [types.InlineQueryResultArticle(
        id=hashlib.md5(f'{link["id"]}'.encode()).hexdigest(),
        title=f'{link["title"]}',
        url=f'{SITE}{link["url_suffix"]}',
        thumb_url=f'{link["thumbnails"][0]}',
        input_message_content=types.InputMessageContent(
            message_text=f'{SITE}{link["url_suffix"]}'
        )
    ) for link in links]
    await query.answer(articles, cache_time=60, is_personal=True)


@dp.message_handler(commands=['start', 'help'])
async def start(message: types.Message):
    save_logs(message)
    answer = await bot.send_message(message.from_user.id, START_TEXT.format(message.from_user.first_name))
    save_logs(answer)


@dp.message_handler(content_types=['text'])
async def get_text(message):
    save_logs(message)
    if SITE not in message.text:
        answer = await bot.send_message(message.chat.id, ERR_TEXT)
        save_logs(answer)
    else:
        markup = types.InlineKeyboardMarkup(row_width=3)
        markup.add(types.InlineKeyboardButton("Видео", callback_data='video'),
                   types.InlineKeyboardButton("Аудио", callback_data='audio'),
                   types.InlineKeyboardButton("Аудио и Видео", callback_data='both'),
                   types.InlineKeyboardButton("Отмена", callback_data='cancel'))
        await bot.send_message(chat_id=message.chat.id,
                               text=message.text,
                               reply_markup=markup)
        await bot.delete_message(message_id=message.message_id,
                                 chat_id=message.chat.id)


@dp.callback_query_handler()
async def callback_inline(call):
    if call.data in ['video', 'audio', 'both']:
        await send(call.data, call.message)
        await bot.edit_message_text(chat_id=call.message.chat.id,
                                    message_id=call.message.message_id,
                                    text=call.message.text,
                                    reply_markup=None)
    if call.data == 'cancel':
        await bot.delete_message(chat_id=call.message.chat.id,
                                 message_id=call.message.message_id)


executor.start_polling(dp, skip_updates=True)
