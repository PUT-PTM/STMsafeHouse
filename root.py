#! /usr/bin/env python2
# -*- coding: utf-8 -*-

import sqlite3

# utworzenie połączenia z bazą przechowywaną na dysku
# lub w pamięci (':memory:')
con = sqlite3.connect('base.db')

# dostęp do kolumn przez indeksy i przez nazwy
con.row_factory = sqlite3.Row

# utworzenie obiektu kursora
cur = con.cursor()

# tworzenie tabel
cur.execute("DROP TABLE IF EXISTS uzytkownicy;")

cur.execute("""
    CREATE TABLE IF NOT EXISTS uzytkownicy (
        id INTEGER PRIMARY KEY ASC,
        login varchar(25) NOT NULL,
        haslo varchar(20) NOT NULL
    )""")

# pobieranie danych z bazy
def czytajdane():
    """Funkcja pobiera i wyświetla dane z bazy."""
    cur.execute(
        """
        SELECT * FROM uzytkownicy
        """)
    uczniowie = cur.fetchall()
    print uczniowie

# wstawiamy jeden rekord danych
cur.execute('INSERT INTO uzytkownicy VALUES(NULL, ?, ?);', ('root', 'toor'))
cur.execute('INSERT INTO uzytkownicy VALUES(NULL, ?, ?);', ('user', 'user123'))

czytajdane()
# wykonujemy zapytanie SQL.
cur.execute('SELECT haslo FROM uzytkownicy WHERE login = ?', ('user',))
klasa_id = cur.fetchone()[0]
print klasa_id

con.close()
