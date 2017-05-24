#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define DB_NAME "library.txt"

// Structure Book Definiton
struct Book {
	char book_ISBN[100];
	char book_title[100];
	char book_author[100];
	int book_edition;
	int book_id;
	int is_Reserved;
};

// clear screen function 
void clrscr(){
    system("@cls||clear");
}


void searchMenu(){
    char isbn[100],
         title[100],
         author[100];
    clrscr();
	int choice;
	puts("Library Management System");
	puts("__________________________\n");
	printf("%15s", "Search Menu");
	puts("");
	puts("");
	puts("1. Search by ISBN");
	puts("2. Search by Book Title");
	puts("3. Search by Author Name");
	puts("4. Return to the main menu");
	puts("");
	puts("");

	do {
		printf("Enter a search menu item >> ");
		scanf("%d", &choice);
		fflush(stdin);
	}
	while(choice > 4 || choice < 1);
	switch(choice){
		case 1:
		    printf("\nEnter ISBN: ");
		    gets(isbn);
			searchBook(1, isbn);
		break;
		case 2:
		    printf("\nEnter Book Title: ");
		    gets(title);
			searchBook(2, title);
		break;
		case 3:
		    printf("\nEnter Author Name: ");
		    gets(author);
			searchBook(3, author);
        break;
        case 4:
            clrscr();
            menu();
		break;
	}

}

void redirect(){
    int choice;
    puts("");
    printf("Press ESC to go back to the menu");
    choice = getch();
    if(choice == 27){
        clrscr();
        menu();
    }
}



void addBook(){
    clrscr();
    int bookId;
    FILE *fp;
    if( (fp = fopen(DB_NAME, "rb+")) == NULL ){
        puts("Database couldn't be opened");
    }
	// ask for information about the book
	struct Book book = {"", "", "", 0, 0, 0}; // for holding data

    printf("Enter Book ID: ");
    scanf("%d", &bookId);

    // check to see if the record already exists
    fseek(fp, (bookId - 1) * sizeof(struct Book), SEEK_SET); // go to that point in file
	fread(&book, sizeof(struct Book), 1, fp); // read that

    if(book.book_id != 0){
        puts("The book with this ID already exists");
	}
	else {
        fflush(stdin); // just to be safe
        printf("Enter Book ISBN: ");
        gets(book.book_ISBN);

        fflush(stdin);
        printf("Enter Book Title: ");
        gets(book.book_title);

        fflush(stdin);
        printf("Enter Book Author: ");
        gets(book.book_author);

        fflush(stdin);
        printf("Enter Book Edition: ");
        scanf("%d", &book.book_edition);


        // assign the book ID we got from the user to the book object
        book.book_id = bookId;
        // write the book
        fseek(fp, (book.book_id - 1) * sizeof(struct Book), SEEK_SET); // go to that point in file
        fwrite(&book, sizeof(struct Book), 1, fp);
	}

    // close the file
    fclose(fp);
    redirect();
}

void deleteBook(){
    clrscr();
    int bookId;
    FILE *fp;
    fp = fopen(DB_NAME, "rb+");
    if( fp == NULL ){
        puts("Database couldn't be opened");
    }
	// ask for information about the book
	struct Book book = {"", "", "", 0, 0, 0}; //for holding data
    struct Book empty = {"", "", "", 0, 0, 0}; //for removing data

    printf("Enter Book ID: ");
    scanf("%d", &bookId);
    // check to see if the record already exists
    fseek(fp, (bookId - 1) * sizeof(struct Book), SEEK_SET); // go to that point in file
	fread(&book, sizeof(struct Book), 1, fp); // read that

    if(book.book_id != 0){
         fseek(fp, (book.book_id - 1) * sizeof(struct Book), SEEK_SET);
         // write empty record there
         fwrite(&empty, sizeof(struct Book), 1, fp);
         puts("Book Successfully Deleted");
	}
	else {
        puts("The book with this ID already doesn't exists");
	}
	fclose(fp);
	redirect();
}

void searchBook(const int mode, const char query[]){
    // clear the screen
    clrscr();
    FILE *fp;
    int result, found = 0;
    if((fp = fopen(DB_NAME, "rb")) == NULL){
        puts("Something wrong with the database");
    }
    struct Book book = {"", "", "", 0, 0, 0}; //for holding data
    puts("");
    printf("%-10s %-16s %-26s %-26s %-16s %-16s\n", "Book ID", "Book ISBN", "Book Title", "Book Author", "Book Edition", "Is Reserved?");
    puts(""); // newline
    switch(mode){
        // search by ISBN
        case 1:
            while(!feof(fp)){
                result = fread(&book, sizeof(struct Book), 1, fp);
                if(result != 0 && book.book_id != 0 && strstr(book.book_ISBN, query) != NULL){
                    found++;
                    printf("%-10d %-16s %-26s %-26s %-16d %-16d\n", book.book_id, book.book_ISBN, book.book_title, book.book_author, book.book_edition, book.is_Reserved);
                }
            }
            if(found == 0){
                puts("No Book with this ISBN Found");
            }
        break;
        case 2:
            while(!feof(fp)){
                result = fread(&book, sizeof(struct Book), 1, fp);
                if(result != 0 && book.book_id != 0 && strstr(book.book_title, query) != NULL){
                    found++;
                    printf("%-10d %-16s %-26s %-26s %-16d %-16d\n", book.book_id, book.book_ISBN, book.book_title, book.book_author, book.book_edition, book.is_Reserved);
                }
            }
            if(found == 0){
                puts("No Book with this Title Found");
            }
        break;
        case 3:
            while(!feof(fp)){
                result = fread(&book, sizeof(struct Book), 1, fp);
                if(result != 0 && book.book_id != 0 && strstr(book.book_author, query) != NULL){
                    found++;
                    printf("%-10d %-16s %-26s %-26s %-16d %-16d\n", book.book_id, book.book_ISBN, book.book_title, book.book_author, book.book_edition, book.is_Reserved);
                }
            }
            if(found == 0){
                puts("No Book with this Author Found");
            }
        break;

    }

    fclose(fp);
    redirect();
}

void updateBook(){
    clrscr();
    int bookId;
    FILE *fp;
    if( (fp = fopen(DB_NAME, "rb+")) == NULL ){
        puts("Database couldn't be opened");
    }
	// ask for information about the book
	struct Book book = {"", "", "", 0, 0, 0}; // for holding data
    struct Book newBook = {"", "", "", 0, 0, 0};
    printf("Enter Book ID: ");
    scanf("%d", &bookId);

    // check to see if the record already exists
    fseek(fp, (bookId - 1) * sizeof(struct Book), SEEK_SET); // go to that point in file
	fread(&book, sizeof(struct Book), 1, fp); // read that

    if(book.book_id != 0){
        fflush(stdin); // just to be safe
        printf("Enter Book ISBN: ");
        gets(newBook.book_ISBN);

        fflush(stdin);
        printf("Enter Book Title: ");
        gets(newBook.book_title);

        fflush(stdin);
        printf("Enter Book Author: ");
        gets(newBook.book_author);

        fflush(stdin);
        printf("Enter Book Edition: ");
        scanf("%d", &newBook.book_edition);


        // assign the book ID we got from the user to the book object
        newBook.book_id = bookId;
        // write the book
        fseek(fp, (newBook.book_id - 1) * sizeof(struct Book), SEEK_SET); // go to that point in file
        fwrite(&newBook, sizeof(struct Book), 1, fp);

        puts("Book Data Updated Successfully");
	}
	else {
        puts("The book with this ID already doesn't exists");
	}
    fclose(fp);
	redirect();
}

void displayAllBooks(){
    clrscr();
    FILE *fp;
    int result;
    if((fp = fopen(DB_NAME, "rb")) == NULL){
        puts("Something wrong with the database");
    }
    struct Book book;
    puts("");
    printf("%-10s %-16s %-26s %-26s %-16s %-16s\n", "Book ID", "Book ISBN", "Book Title", "Book Author", "Book Edition", "Is Reserved?");
    puts(""); // newline
    while(!feof(fp)){
        result = fread(&book, sizeof(struct Book), 1, fp);

        if(result != 0 && book.book_id != 0){
            printf("%-10d %-16s %-26s %-26s %-16d %-16d\n", book.book_id, book.book_ISBN, book.book_title, book.book_author, book.book_edition, book.is_Reserved);
        }

    }
    fclose(fp);
    redirect();
}

void issueBook(){
    clrscr();
    int bookId;
    FILE *fp;
    if( (fp = fopen(DB_NAME, "rb+")) == NULL ){
        puts("Database couldn't be opened");
    }
	// ask for information about the book
	struct Book book = {"", "", "", 0, 0, 0}; // for holding data
    printf("Enter Book ID which is to be issued?: ");
    scanf("%d", &bookId);

    // check to see if the record already exists
    fseek(fp, (bookId - 1) * sizeof(struct Book), SEEK_SET); // go to that point in file
	fread(&book, sizeof(struct Book), 1, fp); // read that
	if(book.book_id != 0){
        book.is_Reserved = 1;

        fseek(fp, (book.book_id - 1) * sizeof(struct Book), SEEK_SET); // go to that point in file
        fwrite(&book, sizeof(struct Book), 1, fp);

        puts("Book Issued Successfully");
	}
	else {
        puts("No book exists with this ID");
	}

	fclose(fp);
	redirect();
}

int menu(){
    clrscr();
	int choice;
	puts("Library Management System");
	puts("__________________________\n");
	printf("%15s", "MENU");
	puts("");
	puts("");
	puts("1. Add Book");
	puts("2. Delete Book");
	puts("3. Search Book");
	puts("4. Update Book");
	puts("5: Display All the Books");
	puts("6: Issue Book");
	puts("7: Exit");
	puts("");
	puts("");

	do {
		printf("Enter a menu item >> ");
		scanf("%d", &choice);
		fflush(stdin);
	}
	while(choice > 7 || choice < 1);
	switch(choice){
		case 1:
			addBook();
			return 0;
		break;
		case 2:
			deleteBook();
		break;
		case 3:
			searchMenu();
		break;
		case 4:
			updateBook();
		break;
		case 5:
			displayAllBooks();
		break;
		case 6:
			issueBook();
		break;
		case 7:
			puts("Bye!");
			exit(0);
		break;
	}

    return choice;
}

int main(){
	menu();
	return 0;
}
