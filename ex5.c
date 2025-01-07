#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// signatures
# define TRUE 1
# define FALSE 0

# define WATCH_PLAYLIST '1'
# define ADD_PLAYLIST '2'
# define REMOVE_PLAYLIST '3'
# define MAIN_EXIT '4'

# define SHOW_PLAYLIST '1'
# define ADD_SONG '2'
# define DELETE_SONG '3'
# define SORT_PLAYLIST '4'
# define PLAY_SONG '5'
# define PLAYLIST_EXIT '6'

# define SONG_QUIT 0

# define SORT_YEAR 1
# define SORT_ASCENDING 2
# define SORT_DESCENDING 3
# define SORT_ALPHABETICALLY 4

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
    SongList *songList;
    int songsNum;
} Playlist;

typedef struct PlaylistList {
    Playlist *playlist;
    struct PlaylistList *next;
} PlaylistList;

char *getString() {
    char *userInput = malloc(sizeof(char) * 2);
    if (userInput == NULL) {
        printf("memory allocation failed\n");
        exit(1);
    }
    char *tempBuffer;

    // Get a single char each time
    char newChar = (char) getchar();
    int counter = 0;
    while (newChar != '\n') {
        // Skip \r chars
        if (newChar == '\r') {
            newChar = (char) getchar();
            continue;
        }
        // Add new char to current index
        userInput[counter] = newChar;
        userInput[counter + 1] = '\0';
        ++counter;

        // Resize the userInput to have space for the new char
        tempBuffer = (char *) realloc(userInput, counter + 2);
        if (tempBuffer == NULL) {
            printf("memory allocation failed\n");
            exit(1);
        }
        userInput = tempBuffer;

        // read the next char
        newChar = (char) getchar();
    }
    return userInput;
}

void printPlaylists(PlaylistList *playlists, int numOfPlaylists) {
    int counter = 1;
    // Point to the first playlist
    PlaylistList *currentPlaylist = playlists;
    while (counter != numOfPlaylists + 1) {
        printf("%d. %s\n", counter, currentPlaylist->playlist->name);
        ++counter;
        if (currentPlaylist->next == NULL) {
            break;
        }
        currentPlaylist = currentPlaylist->next;
    }
    printf("%d. Back to main menu\n", counter);
}

void freeSong(Song *song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

void freeSongList(SongList *songList) {
    SongList *currentNode = songList;
    printf("Trying to get next\n");
    SongList *nextNode = NULL;
    if (currentNode->next != NULL)
        nextNode = currentNode->next;
    while (nextNode != NULL) {
        printf("Inside freeSongList while loop\n");
        freeSong(currentNode->song);
        free(currentNode);
        currentNode = nextNode;
        nextNode = currentNode->next;
    }
    printf("freeing current song node\n");
    if (currentNode->song != NULL)
        freeSong(currentNode->song);
    free(currentNode);
}

void freePlaylist(Playlist *playlist) {
    if (playlist == NULL) {
        return;
    }
    printf("Entering FreeSongList\n");
    if (playlist->songList != NULL) {
        freeSongList(playlist->songList);
        printf("Done FreeSongList\n");
    }
    free(playlist->name);
    free(playlist);
}

void freePlaylists(PlaylistList *playlists) {
    // Go over all nodes
    PlaylistList *currentNode = playlists;
    PlaylistList *nextNode = currentNode->next;
    while (nextNode != NULL) {
        // Free the playlist
        freePlaylist(currentNode->playlist);

        // Free the current node. move to next one.
        free(currentNode);
        currentNode = nextNode;
        nextNode = currentNode->next;
    }
    freePlaylist(currentNode->playlist);
    free(currentNode);
}

// Return the last node of the playlists' list
SongList *findLastSong(SongList *songList) {
    if (songList == NULL) {
        return songList;
    }
    SongList *currentNode = songList;
    while (currentNode->next != NULL) {
        printf("entered loop");
        currentNode = currentNode->next;
    }
    return currentNode;
}

SongList *findSongByIndex(SongList *songList, int index) {
    // Using indexes starting with 1.
    SongList *currentNode = songList;
    int counter = 1;
    while (counter != index) {
        currentNode = currentNode->next;
        ++counter;
    }
    return currentNode;
}

void playSong(Song *song) {
    printf("Now playing %s:\n", song->title);
    printf("$ %s $\n", song->lyrics);
    ++song->streams;
}

void printPlaylist(Playlist *playlist) {
    int counter = 1;
    SongList *currentNode = playlist->songList;
    Song *song = currentNode->song;
    while (song != NULL) {
        printf("%d. Title: %s\n", counter, song->title);
        printf("\tArtist: %s\n", song->artist);
        printf("\tReleased: %d\n", song->year);
        printf("\tStreams: %d\n", song->streams);
        ++counter;
        currentNode = currentNode->next;
        if (currentNode == NULL)
            break;
        song = currentNode->song;
    }
}

void watchPlaylist(Playlist *playlist) {
    printPlaylist(playlist);

    // Assuming input is valid
    int choice = -1;
    while (choice != SONG_QUIT) {
        printf("choose a song to play, or 0 to quit:\n");

        scanf(" %d", &choice);
        printf("got choice\n");
        getchar(); // Consume \n
        if (choice == SONG_QUIT) {
            printf("entered if\n");
            return;
        }

        SongList *songNode = findSongByIndex(playlist->songList, choice);
        playSong(songNode->song);
    }
}

void addSong(Playlist *playlist) {
    SongList *lastNode = findLastSong(playlist->songList);
    SongList *newNode;
    if (lastNode->song != NULL) {
        // If current node is occupied, we add the new node as next
        newNode = calloc(1, sizeof(SongList));
        if (newNode == NULL) {
            printf("memory allocation failed\n");
            exit(1);
        }
        lastNode->next = newNode;
    } else {
        // If current node is empty, this is the node we are editing.
        newNode = lastNode;
    }
    // Create emtpy song
    newNode->song = calloc(1, sizeof(Song));
    if (newNode == NULL) {
        printf("memory allocation failed\n");
        exit(1);
    }

    // Get details from user
    printf("Enter song's details\n");
    printf("Title:\n");
    char *title = getString();
    newNode->song->title = title;

    printf("Artist:\n");
    char *artist = getString();
    newNode->song->artist = artist;

    printf("Year of release:\n");
    int year = 0;
    scanf("%4d", &year);
    newNode->song->year = year;

    getchar(); // Consume \n
    printf("Lyrics:\n");
    char *lyrics = getString();
    newNode->song->lyrics = lyrics;
}

void playAllSongs(Playlist *playlist) {
    SongList *currentSong = playlist->songList;
    while (currentSong->next != NULL) {
        playSong(currentSong->song);
        currentSong = currentSong->next;
    }
    playSong(currentSong->song);
}

void swapFirstSongNode(Playlist *playlist, SongList *currentNode, SongList *nextNode) {
    playlist->songList = nextNode;
    currentNode->next = nextNode->next;
    nextNode->next = currentNode;
}

void swapSongNode(SongList *prevNode, SongList *currentNode, SongList *nextNode) {
    prevNode->next = nextNode;
    currentNode->next = nextNode->next;
    nextNode->next = currentNode;
}

int shouldSwap(int sortType, SongList *currentNode, SongList *nextNode) {
    if (sortType == SORT_ALPHABETICALLY) {
        char *nameCurrent = currentNode->song->title;
        char *nameNext = nextNode->song->title;
        if (strcmp(nameCurrent, nameNext) > 0)
            return TRUE;
        return FALSE;
    }
    if (sortType == SORT_YEAR) {
        if (currentNode->song->year > nextNode->song->year)
            return TRUE;
        return FALSE;
    }
    if (sortType == SORT_ASCENDING) {
        if (currentNode->song->streams > nextNode->song->streams)
            return TRUE;
        return FALSE;
    }
    if (currentNode->song->streams < nextNode->song->streams)
        return TRUE;
    return FALSE;
}

void sortPlaylist(Playlist *playlist, int sortType) {
    int sorted = FALSE, firstSong = TRUE;
    SongList *currentNode = NULL;
    SongList *prevNode = NULL;
    SongList *nextNode = NULL;
    while (!sorted) {
        // Re-assign all values
        sorted = TRUE;
        firstSong = TRUE;
        currentNode = playlist->songList;
        nextNode = currentNode->next;
        prevNode = NULL;

        // Stop when there is no next node
        while (currentNode->next != NULL) {
            // Check if alphabetically the names should be swapped.
            if (shouldSwap(sortType, currentNode, nextNode)) {
                sorted = FALSE;
                if (firstSong) {
                    swapFirstSongNode(playlist, currentNode, nextNode);
                } else {
                    swapSongNode(prevNode, currentNode, nextNode);
                }
            }
            firstSong = FALSE;

            // Go to next node
            prevNode = currentNode;
            currentNode = nextNode;
            nextNode = nextNode->next;
        }
    }
}

void sortMenu(Playlist *playlist) {
    printf("choose:\n1. sort by year\n2. sort by streams - ascending order\n"
        "3. sort by streams - descending order\n4. sort alphabetically\n");
    int choice;
    scanf(" %d", &choice);
    if (choice < 1 || choice > SORT_ALPHABETICALLY) {
        choice = SORT_ALPHABETICALLY;
    }
    sortPlaylist(playlist, choice);
    printf("sorted\n");
}

void deleteSong(Playlist *playlist) {
    printPlaylist(playlist);
    printf("choose a song to delete, or 0 to quit:");
    int choice;
    SongList *tempSong;
    scanf("%d", &choice);
    if (choice == 0) {
        return;
    }
    if (choice == 1) {
        tempSong = playlist->songList;
        playlist->songList = tempSong->next;
        tempSong->next = NULL;
        freeSongList(tempSong);
    } else {
        int index = 2;
        SongList *prevSong = playlist->songList;
        tempSong = prevSong->next;
        while (index < choice) {
            prevSong = tempSong;
            tempSong = tempSong->next;
        }
        // Skip the removed song
        prevSong->next = tempSong->next;
        freeSongList(tempSong);
    }
    printf("Song deleted successfully.");
}

void playlistMenu(Playlist *playlist) {
    printf("playlist %s\n", playlist->name);

    // Init with invalid value
    char choice = '0';
    while (choice != PLAYLIST_EXIT) {
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
        scanf(" %c", &choice);
        getchar(); // Consume \n

        switch (choice) {
            case WATCH_PLAYLIST: {
                watchPlaylist(playlist);
                break;
            }
            case ADD_SONG: {
                addSong(playlist);
                break;
            }
            case DELETE_SONG: {
                deleteSong(playlist);
                break;
            }
            case PLAY_SONG: {
                playAllSongs(playlist);
                break;
            }
            case SORT_PLAYLIST: {
                sortMenu(playlist);
                break;
            }
            case PLAYLIST_EXIT: {
                return;
            }
            default: {
                printf("Invalid Choice\n");
                scanf(" %c", &choice);
                break;
            }
        }
    }
}

PlaylistList *findPlaylistByIndex(PlaylistList *playlists, int index) {
    // Using indexes starting with 1.
    PlaylistList *currentNode = playlists;
    int counter = 1;
    while (counter != index) {
        currentNode = currentNode->next;
        ++counter;
    }
    return currentNode;
}

void watchPlaylists(PlaylistList *playlists, int numOfPlaylists) {
    int choice = 0, exitOption = numOfPlaylists+1;
    while (choice != exitOption + 1) {
        printPlaylists(playlists, numOfPlaylists);

        scanf(" %d", &choice);
        while (choice < 0 || choice > exitOption) {
            printf("Invalid Choice\n");
            scanf(" %d", &choice);
        }
        // Consume \n
        getchar();
        if (choice == exitOption) {
            return;
        }
        PlaylistList *playlistNode = findPlaylistByIndex(playlists, choice);
        playlistMenu(playlistNode->playlist);
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


void addPlaylist(PlaylistList *playlists, int *numOfPlaylists) {
    // Consume \n
    getchar();

    printf("Enter playlist's name:");

    // Start with a string in the len of 1
    char *playlistName = getString();

    // Create new playlist.
    Playlist *newPlaylist = malloc(sizeof(Playlist));
    newPlaylist->name = playlistName;
    newPlaylist->songsNum = 0;
    newPlaylist->songList = calloc(1, sizeof(SongList));
    if (newPlaylist->songList == NULL) {
        printf("memory allocation failed\n");
        exit(1);
    }

    PlaylistList *lastPlaylist;

    if (playlists->playlist == NULL)
        playlists->playlist = newPlaylist;
    else {
        lastPlaylist = findLastPlaylist(playlists);
        PlaylistList *newNode = calloc(1, sizeof(PlaylistList));
        if (newNode == NULL) {
            printf("memory allocation failed\n");
            exit(1);
        }
        newNode->playlist = newPlaylist;
        newNode->next = NULL;

        // Add node to playlists
        lastPlaylist->next = newNode;
    }
    ++*numOfPlaylists;
}

PlaylistList *removePlaylist(PlaylistList *playlists, int numOfPlaylists) {
    // Print menu
    printf("Choose a playlist:\n");
    printPlaylists(playlists, numOfPlaylists);

    int choice, lastChoice = 1;
    scanf(" %d", &choice);
    while (choice < 0 || choice > lastChoice) {
        printf("Invalid Choice\n");
        scanf(" %d", &choice);
        getchar(); // Consume \n
    }

    // User chose to exit
    if (choice == lastChoice) {
        return playlists;
    }
    PlaylistList *tempPlaylist;

    // Remove first playlist
    if (choice == 1) {
        // Point to the playlist that is about to be deleted
        tempPlaylist = playlists;

        // The beginning of the list should be the 2nd playlist
        playlists = playlists->next;

        // Unchained the removed playlist
        tempPlaylist->next = NULL;
        freePlaylists(tempPlaylist);
        free(tempPlaylist);
    } else {
        int index = 2;
        PlaylistList *prevPlaylist = playlists;
        tempPlaylist = prevPlaylist->next;

        // Find index of playlist inputted by user
        while (index < choice) {
            prevPlaylist = tempPlaylist;
            tempPlaylist = tempPlaylist->next;
        }
        // Skip the removed song
        prevPlaylist->next = tempPlaylist->next;
        freePlaylists(tempPlaylist);
    }
    if (playlists == NULL) {
        playlists = calloc(1, sizeof(PlaylistList));
        if (playlists == NULL) {
            printf("memory allocation failed\n");
            exit(1);
        }
        // playlists->numOfPlaylist = 0;
        playlists->playlist = NULL;
        printf("aaa adress %p", playlists);
    }
    printf("Playlist deleted.\n");
    return playlists;
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

int main() {
    // Set the default value of choice to a null one.
    char choice = '0';
    int numOfPlaylists = 0;
    PlaylistList *playlists = calloc(1, sizeof(PlaylistList));
    if (playlists == NULL) {
        printf("memory allocation failed\n");
        return 1;
    }

    while (choice != MAIN_EXIT) {
        printPlaylistsMenu();
        scanf(" %c", &choice);
        switch (choice) {
            case WATCH_PLAYLIST: {
                watchPlaylists(playlists, numOfPlaylists);
                break;
            }
            case ADD_PLAYLIST: {
                addPlaylist(playlists, &numOfPlaylists);
                printf("num of playlists%d\n", numOfPlaylists);
                break;
            }
            case REMOVE_PLAYLIST: {
                playlists = removePlaylist(playlists, numOfPlaylists);
                printf("\naa address %p\n", playlists);
                break;
            }
            case MAIN_EXIT:
                break;
            default: {
                printf("Invalid choice\n");
                break;
            }
        }
    }
    freePlaylists(playlists);
    printf("Done freePlaylists\n");
    printf("Goodbye!\n");
}
