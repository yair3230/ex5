#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL 0

typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct SongList {
    Song *song;
    struct SongList *next;
} SongList;

typedef struct Playlist {
    char *name;
    SongList **songList;
    int songsNum;
} Playlist;

typedef struct PlaylistList {
    Playlist *playlist;
    int numOfPlaylist;
    struct PlaylistList *next;
} PlaylistList;

void watchPlaylists(PlaylistList *playlists) {
    int currentChoice = 1;

    // Point to the first playlist
    PlaylistList *currentPlaylist = playlists;
    for (int i = 0; i < playlists->numOfPlaylist; ++i) {
        // Print the name of current playlist, advance pointer.
        printf("%d. %s\n", currentChoice, currentPlaylist->playlist->name);
        currentPlaylist = currentPlaylist->next;
        ++currentChoice;
    }
    printf("%d. Back to main menu\n", currentChoice);
    char choice;
    scanf(" %c", &choice);
    while (choice < 0 || choice > currentChoice) {
        printf("Invalid Choice\n");
    }
}

// Return the last node of the playlists' list
PlaylistList *findLastPlaylist(PlaylistList *playlists) {
    PlaylistList *currentNode = playlists;
    while (currentNode->next != NULL) {
        printf("entered loop");
        currentNode = currentNode->next;
    }
    return currentNode;
}

void AddPlaylist(PlaylistList *playlists) {
    // Consume \n
    getchar();

    printf("Enter playlist's name:");

    // Start with a string in the len of 1
    char *playlistName = calloc(1, sizeof(char *));

    // Get a single char each time
    char newChar = (char) getchar();
    int counter = 0;
    while (newChar != '\n') {

        // Add new char to current index
        playlistName[counter] = newChar;
        ++counter;

        // Resize the string to have space for the new char
        playlistName = realloc(playlistName, sizeof(playlistName) + sizeof(char*));

        // If realloc failed, crash
        if (playlistName == NULL)
            exit(1);

        // read the next char
        newChar = (char) getchar();
    }

    printf("got name %s\n", playlistName);

    // Get last node.
    PlaylistList *lastPlaylist = findLastPlaylist(playlists);

    // Create new playlist.
    Playlist* newPlaylist= malloc(sizeof(PlaylistList));
    newPlaylist->name = playlistName;
    newPlaylist->songsNum = 0;

    // Add node to playlists
    lastPlaylist->playlist = newPlaylist;
    ++playlists->numOfPlaylist;
}

void deleteSong() {
    printf("Song deleted successfully.\n");
}


void playSong() {
}


void freeSong() {
}

void freePlaylist(P) {
}

void freePlaylists(PlaylistList *playlists) {
    PlaylistList *currentNode = playlists;
    PlaylistList *nextNode = currentNode->next;
    while (nextNode != NULL) {
        free(currentNode);
        currentNode = nextNode;
        nextNode = currentNode->next;
    }
    free(currentNode);
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}


void sortPlaylist() {
    printf("sorted\n");
}


int main() {
    // Set the default value of choice to a null one.
    char choice = '0';
    PlaylistList *playlists = malloc(sizeof(PlaylistList));
    if (playlists == NULL) {
        return 1;
    }

    // Initialize the playlists' list.
    playlists->numOfPlaylist = 0;
    playlists->playlist->name = "First song";
    playlists->next = NULL;

    while (choice != '4') {
        printPlaylistsMenu();
        scanf(" %c", &choice);
        switch (choice) {
            case '1': {
                printf("%s\n",playlists->playlist->name);
                watchPlaylists(playlists);
                break;
            }
            case '2': {
                AddPlaylist(playlists);
                printf("%s\n",playlists->playlist->name);
                break;
            }
            default: {
                printf("Invalid choice\n");
                break;
            }
        }
    }
    freePlaylists(playlists);
    printf("Goodbye!\n");
}
