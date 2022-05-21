import os

from pytube import YouTube
from youtube_search import YoutubeSearch

from config import *
from logger import save_logs


def small(stream):
    return stream.filesize_approx < MAX_SIZE


def searcher(text):
    return YoutubeSearch(text, max_results=10).to_dict()


async def send(type, message):
    if type == 'both':
        await send('audio', message)
        await send('video', message)
    else:
        yt = YouTube(message.text)
        streams = yt.streams.filter(type=type, custom_filter_functions=[small])
        answer = await bot.send_message(message.chat.id, WAIT_TEXT)
        save_logs(answer)
        if not streams:
            err = await bot.send_message(message.chat.id, FILE_SIZE_ERRTXT.format(type))
            save_logs(err)
        else:
            if type == 'audio':
                obj = streams.get_audio_only()
                obj.download(filename=f'audio{message.date.strftime("%d%m%Y%H%M%S")}.mp3')
                audio = await bot.send_audio(message.chat.id,
                                             open(f'audio{message.date.strftime("%d%m%Y%H%M%S")}.mp3', "rb"),
                                             duration=yt.length,
                                             title=yt.title,
                                             thumb=yt.thumbnail_url)
                save_logs(audio)
                os.remove(f'audio{message.date.strftime("%d%m%Y%H%M%S")}.mp3')
            elif type == 'video':
                obj = streams.get_highest_resolution()
                obj.download(filename=f'video{message.date.strftime("%d%m%Y%H%M%S")}.mp3')
                video = await bot.send_video(message.chat.id,
                                             open(f'video{message.date.strftime("%d%m%Y%H%M%S")}.mp3', "rb"),
                                             duration=yt.length,
                                             caption=yt.title,
                                             thumb=yt.thumbnail_url)
                save_logs(video)
                os.remove(f'video{message.date.strftime("%d%m%Y%H%M%S")}.mp3')
        await bot.delete_message(message_id=answer.message_id,
                                 chat_id=answer.chat.id)
