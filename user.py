# -*- coding: utf-8 -*-
import sys
import sqlite3

#login = sys.argv[1]
#password = sys.argv[2]

# utworzenie połączenia z bazą przechowywaną na dysku
# lub w pamięci (':memory:')
con = sqlite3.connect('base.db')

# dostęp do kolumn przez indeksy i przez nazwy
con.row_factory = sqlite3.Row

# utworzenie obiektu kursora
cur = con.cursor()

# pobieranie danych z bazy
def czytajdane():
    """Funkcja pobiera i wyświetla dane z bazy."""
    cur.execute(
        """
        SELECT * FROM uzytkownicy
        """)
    uczniowie = cur.fetchall()
    print uczniowie

czytajdane()

a = cur.execute('SELECT haslo FROM uzytkownicy WHERE login = ?', ('user',))
con.close()
