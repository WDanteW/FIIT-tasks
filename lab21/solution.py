#!/usr/bin/env python3

import os
import argparse

def main():
    parser = argparse.ArgumentParser(
        description="Конкатенация файла с ограничением размера"
    )

    parser.add_argument("file", nargs='?', help="Путь к файлу")
    parser.add_argument("count", nargs='?', type=int, help="Количество повторов")
    parser.add_argument("limit", nargs='?', type=int, help="Лимит в байтах")

    args = parser.parse_args()

    file = args.file
    count = args.count
    limit = args.limit

    # Ввод, если не передано
    if file is None:
        file = input("Введите имя файла: ")

    if count is None:
        count = int(input("Введите количество повторов: "))

    if limit is None:
        limit = int(input("Введите лимит (в байтах): "))

    # Проверки
    if not os.path.isfile(file):
        print("Ошибка: файл не найден")
        parser.print_help()
        return

    if count < 0 or limit < 0:
        print("Ошибка: значения должны быть неотрицательными")
        parser.print_help()
        return

    size = os.path.getsize(file)

    # Имя файла
    filename = os.path.basename(file)
    output = f"result_{filename}"

    current_size = 0

    with open(output, "wb") as out:
        for _ in range(count):
            if current_size + size > limit:
                break

            with open(file, "rb") as f:
                out.write(f.read())

            current_size += size

    print(f"Готово: создан файл {output} (размер: {current_size} байт)")


if __name__ == "__main__":
    main()
