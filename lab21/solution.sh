#!/bin/bash

show_help() {
    echo "Использование: $0 [файл] [число_повторов] [лимит_байт]"
    echo
    echo "Скрипт повторяет содержимое файла заданное количество раз,"
    echo "но итоговый размер не превышает лимит."
    echo
    echo "Параметры:"
    echo "  файл              — путь к файлу"
    echo "  число_повторов    — сколько раз повторить"
    echo "  лимит_байт        — максимальный размер результата"
}

# Обработка help
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    show_help
    exit 0
fi

file=$1
count=$2
limit=$3

# Ввод, если аргументы не заданы
if [[ -z "$file" ]]; then
    read -p "Введите имя файла: " file
fi

if [[ -z "$count" ]]; then
    read -p "Введите количество повторов: " count
fi

if [[ -z "$limit" ]]; then
    read -p "Введите лимит (в байтах): " limit
fi

# Проверки
if [[ ! -f "$file" ]]; then
    echo "Ошибка: файл не найден"
    show_help
    exit 1
fi

if ! [[ "$count" =~ ^[0-9]+$ ]] || ! [[ "$limit" =~ ^[0-9]+$ ]]; then
    echo "Ошибка: count и limit должны быть числами"
    show_help
    exit 1
fi

# Размер файла
size=$(stat -c%s "$file")

# Имя выходного файла (FIX!)
filename=$(basename "$file")
output="result_$filename"

> "$output"

current_size=0

for ((i=0; i<count; i++)); do
    if (( current_size + size > limit )); then
        break
    fi

    cat "$file" >> "$output"
    current_size=$((current_size + size))
done

echo "Готово: создан файл $output (размер: $current_size байт)"
