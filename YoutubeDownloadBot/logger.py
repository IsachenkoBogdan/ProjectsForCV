from config import connection


def save_logs(message):
    with open('logs.txt', 'a', encoding='utf-8') as logs:
        end = '\n'
        video = None
        audio = None
        if message.audio is not None:
            end = f' with audio {message.audio.title}.\n'
            audio = message.audio.title
        elif message.video is not None:
            end = f' with video {message.video.file_name}.\n'
            video = message.video.file_name
        logs.write(f'"{message.text}" was sent from '
                   f'@{message.from_user.username} in '
                   f'{message.chat.id} on '
                   f'{message.date}' + end)

        cursor = connection.cursor()
        insert_query = '''INSERT INTO messages
        (message_id, date, user_id, user_name,chat_id, message_text, video, audio)
        VALUES 
        (?, ?, ?, ?, ?, ?, ?, ?)'''
        data = (message.message_id,
                message.date,
                message.from_user.id,
                message.from_user.username,
                message.chat.id,
                message.text,
                video,
                audio)
        cursor.execute(insert_query, data)
        connection.commit()
        cursor.close()
