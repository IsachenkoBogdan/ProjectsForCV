import sqlite3

from config import create_table_users, connection


def create_table():
    curs = connection.cursor()
    try:
        curs.execute(create_table_users)
        connection.commit()
    except sqlite3.Error:
        pass
    curs.close()
