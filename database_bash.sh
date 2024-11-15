declare -A student_db
show_menu() {
    echo "Select an operation:"
    echo "a) Insert a record"
    echo "b) Delete a record"
    echo "c) Update a record"
    echo "d) Search for a record"
    echo "e) Exit"
    read -p "Enter your choice: " choice
}

insert_record() {
    read -p "Enter Student ID: " id
    read -p "Enter Student Name: " name
    read -p "Enter Student Grade: " grade
    student_db["$id"]="$name,$grade"
    echo "Record inserted successfully."
}

delete_record() {
    read -p "Enter Student ID to delete: " id
    if [[ -n "${student_db[$id]}" ]]; then
        unset student_db["$id"]
        echo "Record with ID $id deleted successfully."
    else
        echo "Record with ID $id not found."
    fi
}

update_record() {
    read -p "Enter Student ID to update: " id
    if [[ -n "${student_db[$id]}" ]]; then
        read -p "Enter new Student Name: " name
        read -p "Enter new Student Grade: " grade
        student_db["$id"]="$name,$grade"
        echo "Record with ID $id updated successfully."
    else
        echo "Record with ID $id not found."
    fi
}

search_record() {
    read -p "Enter Student ID to search: " id
    if [[ -n "${student_db[$id]}" ]]; then
        echo "Record found:"
        echo "$id: ${student_db[$id]}"
    else
        echo "Record with ID $id not found."
    fi
}

while true; do
    show_menu
    case $choice in
        a|A) insert_record ;;
        b|B) delete_record ;;
        c|C) update_record ;;
        d|D) search_record ;;
        e|E) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid option. Please try again." ;;
    esac
done
