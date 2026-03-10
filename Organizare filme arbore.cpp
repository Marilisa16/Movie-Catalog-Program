/*
 * Movie Catalog Program
 * This program organizes movies into a binary search tree (BST) by genre.
 * Users can search by title, year, or actor and get recommendations.
 * Author: Floreanu Maria-Luiza
 * Date: 14.01.2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // For randomization

// Structure for a character-actor pair
typedef struct CharacterActor {
    char character[50];
    char actor[50];
    struct CharacterActor *next; // Pointer to the next character-actor pair (linked list)
} CharacterActor;

// Structure to represent a movie node in a binary search tree (BST)
typedef struct MovieNode {
    char title[100];
    int year;
    CharacterActor *characters; // Linked list of characters and actors
    struct MovieNode *left; // Pointer to the left subtree
    struct MovieNode *right; // Pointer to the right subtree
} MovieNode;

// Structure to represent a genre tree, which holds a BST of movies
typedef struct GenreTree {
    char genre[50];
    MovieNode *root;
} GenreTree;

// Function to create a new character-actor pair
CharacterActor* createCharacterActor(char *character, char *actor) {
    CharacterActor *newPair = (CharacterActor*)malloc(sizeof(CharacterActor)); // Allocate memory for when we create a new node
    strcpy(newPair->character, character); // Copy character name
    strcpy(newPair->actor, actor); // Copy actor name
    newPair->next = NULL; // Initialize next pointer to NULL
    return newPair; // Return the created node
}


// Function to create a new movie node with given data
MovieNode* createMovieNode(char *title, int year, CharacterActor *characters) {
    MovieNode *newNode = (MovieNode*)malloc(sizeof(MovieNode)); // Allocate memory
    strcpy(newNode->title, title); // Copy movie title
    newNode->year = year; // Store movie release year
    newNode->characters = characters; // Assign character list
    newNode->left = NULL; // Initialize left child to NULL
    newNode->right = NULL;  // Initialize right child to NULL
    return newNode; // Return created movie node
}


// Function to insert a movie into the binary tree
MovieNode* insertMovie(MovieNode *root, MovieNode *newMovie) {
    if (root == NULL) { // Base case: if tree is empty, return new movie node
        return newMovie;
    }
    if (strcmp(newMovie->title, root->title) < 0) { // If the new movie's title is smaller, insert in the left subtree
        root->left = insertMovie(root->left, newMovie); // Insert into left subtree
    } else { // If the new movie's title is greater, insert in the right subtree
        root->right = insertMovie(root->right, newMovie); // Insert into right subtree
    }
    return root; // Return the modified tree root
}

// Function to create a genre tree
GenreTree* createGenreTree(char *genre) {
    GenreTree *newGenre = (GenreTree*)malloc(sizeof(GenreTree)); // Allocate memory
    strcpy(newGenre->genre, genre); // Copy genre name
    newGenre->root = NULL; // Initialize root as NULL (empty BST)
    return newGenre; // Return created genre tree
}

// Helper function to collect all movies in a tree
void collectMovies(MovieNode *root, MovieNode **movies, int *count) {
    if (root != NULL) {
        collectMovies(root->left, movies, count);
        movies[*count] = root;
        (*count)++;
        collectMovies(root->right, movies, count);
    }
}

// Function to search for a movie by title
MovieNode* searchMovieByTitle(MovieNode *root, char *title) {
    if (root == NULL) { // If tree is empty or movie is not found
        return NULL;
    }
    if (strcmp(root->title, title) == 0) { // If the title matches, return the movie node
        printf("\n--- Movie Found ---\n");
        printf("Title: %s\nYear: %d\nCharacters:\n", root->title, root->year);
        CharacterActor *character = root->characters;
        while (character) {
            printf("  %s - %s\n", character->character, character->actor);
            character = character->next;
        }
        printf("\n");
        return root;
    }
    if (strcmp(title, root->title) < 0) { // If title is smaller, search in left subtree
        return searchMovieByTitle(root->left, title);
    } else { // If title is greater, search in right subtree
        return searchMovieByTitle(root->right, title);
    }
}


// Function to search for movies by year
void searchMovieByYear(MovieNode *root, int year) {
    if (root != NULL) { // If tree is not empty
        searchMovieByYear(root->left, year); // Search left subtree
        if (root->year == year) { // If year matches, print movie details
            printf("\n--- Movie Found ---\n");
            printf("Title: %s\nYear: %d\nCharacters:\n", root->title, root->year);
            CharacterActor *character = root->characters;
            while (character) {
                printf("  %s - %s\n", character->character, character->actor);
                character = character->next;
            }
            printf("\n");
        }
        searchMovieByYear(root->right, year); // Search right subtree
    }
}

// Function to search for movies by actor or character name
void searchMovieByActorOrCharacter(MovieNode *root, char *name) {
    if (root != NULL) { // Base case: if root is NULL, stop recursion
        searchMovieByActorOrCharacter(root->left, name); // Recursively search in the left subtree
        CharacterActor *character = root->characters; // Get the list of characters for the current movie
        while (character) { // Loop through the linked list of characters
            if (strcmp(character->actor, name) == 0 || strcmp(character->character, name) == 0) {  // If the actor or character name matches the searched name
                printf("\n--- Movie Found ---\n");
                printf("Title: %s\nYear: %d\nCharacters:\n", root->title, root->year);
                CharacterActor *charIter = root->characters; // Iterate through all characters in the movie
                while (charIter) {
                    printf("  %s - %s\n", charIter->character, charIter->actor); // Print character-actor pair
                    charIter = charIter->next; // Move to the next character in the linked list
                }
                printf("\n");
                break;
            }
            character = character->next; // Move to the next character in the linked list
        }
        searchMovieByActorOrCharacter(root->right, name); // Recursively search in the right subtree
    }
}


// Function to search movies in a genre
// Function to search movies in a genre
// Function to search for movies in a specific genre
void searchMoviesInGenre(GenreTree *genre) {
    int searchChoice; // Variable to store the user's choice
    char input[100]; // Buffer to store user input
    int year; // Variable to store the year input

    // Display genre-specific search options
    printf("\n--- Search in %s Movies ---\n", genre->genre);
    printf("1. Search by title\n");
    printf("2. Search by year\n");
    printf("3. Search by actor/character\n");
    printf("Choose an option: ");

    scanf("%d", &searchChoice); // Read user choice
    getchar(); // Consume newline character left in the buffer

    switch (searchChoice) {
        case 1: // Search by movie title
            printf("Enter the movie title: ");
            fgets(input, 100, stdin); // Read the title from user input
            input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character

            // Search the movie in the BST and check if it exists
            if (searchMovieByTitle(genre->root, input) == NULL) {
                printf("Movie not found in %s category.\n", genre->genre);
            }
            break;

        case 2: // Search by movie year
            printf("Enter the year: ");
            scanf("%d", &year); // Read the year input
            getchar(); // Consume newline character
            printf("\nMovies from %d:\n\n", year);

            // Search and display movies from the given year
            searchMovieByYear(genre->root, year);
            break;

        case 3: // Search by actor or character name
            printf("Enter the actor's or character's name: ");
            fgets(input, 100, stdin); // Read name input
            input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character
            printf("\nMovies with %s:\n\n", input);

            // Search for movies containing the given actor or character
            searchMovieByActorOrCharacter(genre->root, input);
            break;

        default: // Invalid input handling
            printf("Invalid option. Returning to the genre menu.\n");
    }
}




// Function to display all movies in a random order
void displayAllMoviesRandomly(GenreTree *genres[], int size) {
    int totalMovies = 0; // Variable to count the total number of movies

    // Iterate through all genres and count the total number of movies
    for (int i = 0; i < size; i++) {
        MovieNode *temp = genres[i]->root; // Start at the root of each genre tree
        while (temp != NULL) {
            totalMovies++; // Increment movie count
            temp = temp->left ? temp->left : temp->right; // Traverse through the BST
        }
    }

    // Allocate memory for storing all movie nodes
    MovieNode **allMovies = (MovieNode **)malloc(totalMovies * sizeof(MovieNode *));
    int count = 0; // Variable to keep track of movies added to the array

    // Collect all movies from each genre into the array
    for (int i = 0; i < size; i++) {
        collectMovies(genres[i]->root, allMovies, &count);
    }

    // Randomize the order of movies in the array using the Fisher-Yates shuffle algorithm
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < count; i++) {
        int j = rand() % count; // Generate a random index
        MovieNode *temp = allMovies[i]; // Swap current element with randomly chosen one
        allMovies[i] = allMovies[j];
        allMovies[j] = temp;
    }

    // Display all movies in the shuffled order
    printf("\n--- All Movies (Random Order) ---\n");
    for (int i = 0; i < count; i++) {
        MovieNode *movie = allMovies[i]; // Get the current movie node
        printf("Title: %s\nYear: %d\nCharacters:\n", movie->title, movie->year);
        CharacterActor *character = movie->characters;

        // Print all characters and actors associated with the movie
        while (character) {
            printf("  %s - %s\n", character->character, character->actor);
            character = character->next;
        }
        printf("\n"); // Add space between movies
    }

    free(allMovies); // Free allocated memory for movie storage
}

// Function to display movies from a genre tree in order (in-order traversal)
void displayMovies(MovieNode *root) {
    if (root != NULL) {
        displayMovies(root->left); // Recursively display left subtree

        // Print movie details
        printf("\nTitle: %s\nYear: %d\nCharacters:\n", root->title, root->year);
        CharacterActor *character = root->characters;

        // Iterate through all character-actor pairs
        while (character) {
            printf("  %s - %s\n", character->character, character->actor);
            character = character->next;
        }
        printf("\n");

        displayMovies(root->right); // Recursively display right subtree
    }
}

// Forward declaration of the recommendMovieMenu function
void recommendMovieMenu(GenreTree *genres[], int size);

// Function to allow the user to select a genre and display its movies
void displayMoviesByGenreMenu(GenreTree *genres[], int size) {
    int choice; // Variable to store the user's choice

    // Display available genres
    printf("\nSelect a genre:\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %s\n", i + 1, genres[i]->genre);
    }
    printf("%d. Recommend a random movie\n", size + 1); // Option for random recommendation
    printf("Choose an option: ");
    scanf("%d", &choice);
    getchar(); // Consume newline character to prevent input issues

    // Handle user selection
    if (choice >= 1 && choice <= size) {
        searchMoviesInGenre(genres[choice - 1]); // Display movies from the selected genre
    } else if (choice == size + 1) {
        printf("Recommending a random movie...\n");
    } else {
        printf("Invalid option. Returning to the main menu.\n");
    }
}

// Function to count the number of movies in a genre (recursively counts nodes in the BST)
int countMovies(MovieNode *root) {
    if (root == NULL) {
        return 0; // Base case: no movies in the subtree
    }
    return 1 + countMovies(root->left) + countMovies(root->right); // Count current movie and recurse on subtrees
}


// Function to get a random movie from the binary search tree (BST)
MovieNode* getRandomMovie(MovieNode *root, int index, int *current) {
    if (root == NULL) {
        return NULL; // Base case: If the tree/subtree is empty, return NULL
    }

    // Recursively check the left subtree for the random index
    MovieNode *left = getRandomMovie(root->left, index, current);
    if (left != NULL) return left; // If found in left subtree, return it

    // If the current index matches the target index, return this movie node
    if (*current == index) {
        return root;
    }
    (*current)++; // Increment the index counter

    // Continue searching in the right subtree
    return getRandomMovie(root->right, index, current);
}

// Function to recommend a random movie from a specified genre
void recommendRandomMovie(GenreTree *genre) {
    int movieCount = countMovies(genre->root); // Count total movies in the genre
    if (movieCount == 0) {
        printf("\nNo movies available in this genre.\n");
        return; // Exit if there are no movies
    }

    srand(time(NULL)); // Seed the random number generator
    int randomIndex = rand() % movieCount; // Generate a random index within the total count
    int current = 0; // Initialize the current index counter
    MovieNode *randomMovie = getRandomMovie(genre->root, randomIndex, &current); // Get the random movie

    // Display the randomly selected movie details
    if (randomMovie != NULL) {
        printf("\n--- Recommended Movie ---\n");
        printf("Title: %s\nYear: %d\nCharacters:\n", randomMovie->title, randomMovie->year);
        CharacterActor *character = randomMovie->characters;
        while (character) {
            printf("  %s - %s\n", character->character, character->actor);
            character = character->next;
        }
        printf("\n");
    }
}

// Function to display the recommendation menu where the user can select a genre
void recommendMovieMenu(GenreTree *genres[], int size) {
    int choice; // Variable to store the user's choice

    // Display available genres for recommendation
    printf("\nSelect a genre for recommendation:\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %s\n", i + 1, genres[i]->genre);
    }
    printf("Choose an option: ");
    scanf("%d", &choice);
    getchar(); // Consume newline character to prevent input issues

    // Validate user input and recommend a movie from the selected genre
    if (choice >= 1 && choice <= size) {
        recommendRandomMovie(genres[choice - 1]);
    } else {
        printf("Invalid option. Returning to the main menu.\n");
    }
}


// Function to display the main menu and handle user interactions
void menu(GenreTree *genres[], int size) {
    int choice; // Variable to store user input

    do {
        // Display the main menu options
        printf("\n--- Movie Catalog Menu ---\n");
        printf("1. Display movies by genre\n");  // Option to browse movies by genre
        printf("2. Recommend a random movie\n"); // Option to get a random movie recommendation
        printf("3. Exit\n"); // Exit the program
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character to avoid input issues

        // Handle user choice with a switch statement
        switch (choice) {
            case 1:
                displayMoviesByGenreMenu(genres, size); // Call function to display movies by genre
                break;
            case 2:
                recommendMovieMenu(genres, size); // Call function to recommend a random movie
                break;
            default:
                printf("Invalid option. Try again.\n"); // Handle invalid input
        }
    } while (choice != 3); // Loop until the user selects option 3 (Exit)
}


int main() {
    // Create genre trees
    GenreTree *comedy = createGenreTree("Comedy");
    GenreTree *action = createGenreTree("Action");
    GenreTree *animation = createGenreTree("Animation");

    GenreTree *genres[] = {comedy, action, animation};

    // Add example movies to each genre
    comedy->root = insertMovie(comedy->root, createMovieNode("The Hangover", 2009, createCharacterActor("Alan", "Zach Galifianakis")));
    comedy->root = insertMovie(comedy->root, createMovieNode("Ted", 2012, createCharacterActor("Ted", "Mark Wahlberg")));
    comedy->root = insertMovie(comedy->root, createMovieNode("Superbad", 2007, createCharacterActor("Seth", "Jonah Hill")));
    comedy->root = insertMovie(comedy->root, createMovieNode("Dumb and Dumber", 1994, createCharacterActor("Harry", "Jeff Daniels")));
    comedy->root = insertMovie(comedy->root, createMovieNode("Step Brothers", 2008, createCharacterActor("Brennan", "Will Ferrell")));
    comedy->root = insertMovie(comedy->root, createMovieNode("Anchorman", 2004, createCharacterActor("Ron Burgundy", "Will Ferrell")));

    action->root = insertMovie(action->root, createMovieNode("John Wick", 2014, createCharacterActor("John Wick", "Keanu Reeves")));
    action->root = insertMovie(action->root, createMovieNode("Mission Impossible", 1996, createCharacterActor("Ethan Hunt", "Tom Cruise")));
    action->root = insertMovie(action->root, createMovieNode("Die Hard", 1988, createCharacterActor("John McClane", "Bruce Willis")));
    action->root = insertMovie(action->root, createMovieNode("Mad Max: Fury Road", 2015, createCharacterActor("Furiosa", "Charlize Theron")));
    action->root = insertMovie(action->root, createMovieNode("The Dark Knight", 2008, createCharacterActor("Bruce Wayne", "Christian Bale")));
    action->root = insertMovie(action->root, createMovieNode("Gladiator", 2000, createCharacterActor("Maximus", "Russell Crowe")));

    animation->root = insertMovie(animation->root, createMovieNode("The Lion King", 1994, createCharacterActor("Simba", "Matthew Broderick")));
    animation->root = insertMovie(animation->root, createMovieNode("Frozen", 2013, createCharacterActor("Elsa", "Idina Menzel")));
    animation->root = insertMovie(animation->root, createMovieNode("Toy Story", 1995, createCharacterActor("Woody", "Tom Hanks")));
    animation->root = insertMovie(animation->root, createMovieNode("Finding Nemo", 2003, createCharacterActor("Marlin", "Albert Brooks")));
    animation->root = insertMovie(animation->root, createMovieNode("Shrek", 2001, createCharacterActor("Shrek", "Mike Myers")));
    animation->root = insertMovie(animation->root, createMovieNode("Up", 2009, createCharacterActor("Carl", "Ed Asner")));

    menu(genres, 3);

    return 0;
}
