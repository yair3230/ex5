#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define TRUE 1
# define FALSE 0
# define ERROR_EXIT_CODE 1

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

char *getString();

void freeSong(Song *song);

void freeSongList(SongList *songList, int songNum);

void freePlaylist(Playlist *playlist);

void freePlaylists(PlaylistList *playlists, int numOfPlaylists);

void printPlaylists(PlaylistList *playlists, int numOfPlaylists);

void printPlaylist(Playlist *playlist);

void printPlaylistsMenu();

// Return the last node of the playlists' list
SongList *findLastSong(SongList *songList, int songNum);

SongList *findSongByIndex(SongList *songList, int index);

// Return the last node of the playlists' list
PlaylistList *findLastPlaylist(PlaylistList *playlists, int numOfPlaylists);

PlaylistList *findPlaylistByIndex(PlaylistList *playlists, int index);

void swapFirstSongNode(Playlist *playlist, SongList *currentNode, SongList *nextNode);

void swapSongNode(SongList *prevNode, SongList *currentNode, SongList *nextNode);

int shouldSwap(int sortType, SongList *currentNode, SongList *nextNode);

void sortPlaylist(Playlist *playlist, int sortType);

void sortMenu(Playlist *playlist);

void addSong(Playlist *playlist);

void playSong(Song *song);

void playAllSongs(Playlist *playlist);

void deleteSong(Playlist *playlist);

void watchPlaylist(Playlist *playlist);

void playlistMenu(Playlist *playlist);

void watchPlaylists(PlaylistList *playlists, int numOfPlaylists);

void addPlaylist(PlaylistList *playlists, int *numOfPlaylists);

PlaylistList *removePlaylist(PlaylistList *playlists, int *numOfPlaylists);


int main() {
    // Set the default value of choice to a null one.
    char choice = '0';
    int numOfPlaylists = 0, consumedChar;
    PlaylistList *playlists = calloc(1, sizeof(PlaylistList));
    if (playlists == NULL) {
        printf("memory allocation failed\n");
        return ERROR_EXIT_CODE;
    }

    while (choice != MAIN_EXIT) {
        printPlaylistsMenu();
        scanf(" %c", &choice);

        consumedChar = getchar(); // Consume \n
        if (consumedChar == '\r') {
            getchar();
        }
        switch (choice) {
            case WATCH_PLAYLIST: {
                watchPlaylists(playlists, numOfPlaylists);
                break;
            }
            case ADD_PLAYLIST: {
                addPlaylist(playlists, &numOfPlaylists);
                break;
            }
            case REMOVE_PLAYLIST: {
                playlists = removePlaylist(playlists, &numOfPlaylists);
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
    freePlaylists(playlists, numOfPlaylists);
    printf("Goodbye!\n");
}

char *getString() {
    // Allocate space for a single char and \0
    char *userInput = malloc(sizeof(char) * 2);
    if (userInput == NULL) {
        printf("memory allocation failed\n");
        exit(ERROR_EXIT_CODE);
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
            exit(ERROR_EXIT_CODE);
        }
        userInput = tempBuffer;

        // read the next char
        newChar = (char) getchar();
    }
    return userInput;
}

// free functions
void freeSong(Song *song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

void freeSongList(SongList *songList, int songNum) {
    SongList *currentNode = songList;
    SongList *nextNode = NULL;

    // Set next node if there is any
    if (currentNode->next != NULL)
        nextNode = currentNode->next;
    int index = 0;
    // free current song and move to next, only if a next song exists
    while (index < songNum - 1) {
        freeSong(currentNode->song);
        free(currentNode);
        currentNode = nextNode;
        nextNode = currentNode->next;
        ++index;
    }
    if (currentNode->song != NULL)
        freeSong(currentNode->song);
    free(currentNode);
}

void freePlaylist(Playlist *playlist) {
    if (playlist == NULL) {
        return;
    }
    if (playlist->songList != NULL) {
        freeSongList(playlist->songList, playlist->songsNum);
    }
    free(playlist->name);
    free(playlist);
}

void freePlaylists(PlaylistList *playlists, int numOfPlaylists) {
    // Go over all nodes
    PlaylistList *currentNode = playlists;
    PlaylistList *nextNode = currentNode->next;
    int index = 1;
    while (index < numOfPlaylists) {
        // Free the playlist
        freePlaylist(currentNode->playlist);

        // Free the current node. move to next one.
        free(currentNode);
        currentNode = nextNode;
        nextNode = currentNode->next;
        ++index;
    }
    freePlaylist(currentNode->playlist);
    free(currentNode);
}

// print function
void printPlaylists(PlaylistList *playlists, int numOfPlaylists) {
    printf("Choose a playlist:\n");
    int counter = 1;
    // Point to the first playlist
    PlaylistList *currentPlaylist = playlists;
    while (counter < numOfPlaylists + 1) {
        printf("%d. %s\n", counter, currentPlaylist->playlist->name);
        if (currentPlaylist->next != NULL) {
            currentPlaylist = currentPlaylist->next;
        }
        ++counter;
    }
    printf("%d. Back to main menu\n", counter);
}

void printPlaylist(Playlist *playlist) {
    int counter = 1;
    SongList *currentNode = playlist->songList;
    Song *song = currentNode->song;
    while (counter < playlist->songsNum + 1) {
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

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

// find functions
// Return the last node of the playlists' list
SongList *findLastSong(SongList *songList, int songNum) {
    if (songNum == 0) {
        return songList;
    }
    int counter = 1;
    SongList *currentNode = songList;
    while (counter < songNum) {
        currentNode = currentNode->next;
        ++counter;
    }
    return currentNode;
}

SongList *findSongByIndex(SongList *songList, int index) {
    // Using indexes starting with 1.
    SongList *currentNode = songList;
    int counter = 1;
    while (counter < index) {
        currentNode = currentNode->next;
        ++counter;
    }
    return currentNode;
}

// Return the last node of the playlists' list
PlaylistList *findLastPlaylist(PlaylistList *playlists, int numOfPlaylists) {
    PlaylistList *currentNode = playlists;
    int index = 1;
    while (index < numOfPlaylists) {
        currentNode = currentNode->next;
        ++index;
    }
    return currentNode;
}

PlaylistList *findPlaylistByIndex(PlaylistList *playlists, int index) {
    // Using indexes starting with 1.
    PlaylistList *currentNode = playlists;
    int counter = 1;
    while (counter < index) {
        currentNode = currentNode->next;
        ++counter;
    }
    return currentNode;
}

// Sort functions
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

        // Check if next name should come before current
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
    int isSorted = FALSE, isFirstSong = TRUE;
    SongList *currentNode = NULL;
    SongList *prevNode = NULL;
    SongList *nextNode = NULL;
    while (!isSorted) {
        // Re-assign all values
        isSorted = TRUE;
        isFirstSong = TRUE;
        currentNode = playlist->songList;
        nextNode = currentNode->next;
        prevNode = NULL;

        // Stop when there is no next node
        int index = 1;
        while (index < playlist->songsNum) {
            // Check if alphabetically the names should be swapped.
            if (shouldSwap(sortType, currentNode, nextNode)) {
                isSorted = FALSE;
                if (isFirstSong) {
                    swapFirstSongNode(playlist, currentNode, nextNode);
                } else {
                    swapSongNode(prevNode, currentNode, nextNode);
                }
            }
            isFirstSong = FALSE;

            // Go to next node
            prevNode = currentNode;
            currentNode = nextNode;
            nextNode = nextNode->next;
            ++index;
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

// Song functions
void addSong(Playlist *playlist) {
    SongList *lastNode = findLastSong(playlist->songList, playlist->songsNum);
    SongList *newNode;
    if (lastNode->song != NULL) {
        // If current node is occupied, we add the new node as next
        newNode = calloc(1, sizeof(SongList));
        if (newNode == NULL) {
            printf("memory allocation failed\n");
            exit(ERROR_EXIT_CODE);
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
        exit(ERROR_EXIT_CODE);
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
    int year;
    scanf("%4d", &year);
    newNode->song->year = year;
    int consumedChar = getchar(); // Consume \r\n
    if (consumedChar == '\r') {
        getchar();
    }
    printf("Lyrics:\n");
    char *lyrics = getString();
    newNode->song->lyrics = lyrics;
    ++playlist->songsNum;
}

void playSong(Song *song) {
    printf("Now playing %s:\n", song->title);
    printf("$ %s $\n", song->lyrics);
    ++song->streams;
}

void playAllSongs(Playlist *playlist) {
    SongList *currentSong = playlist->songList;
    int index = 0;
    while (index < playlist->songsNum-1) {
        playSong(currentSong->song);
        currentSong = currentSong->next;
        ++index;
    }
    playSong(currentSong->song);
}

void deleteSong(Playlist *playlist) {
    printPlaylist(playlist);
    printf("choose a song to delete, or 0 to quit:\n");
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
        freeSongList(tempSong, 1);
        if (playlist->songList == NULL) {
            playlist->songList = calloc(1,sizeof(SongList));
        }
    } else {
        int index = 2;
        SongList *prevSong = playlist->songList;
        tempSong = prevSong->next;

        // Find the song we want to remove
        while (index < choice) {
            prevSong = tempSong;
            tempSong = tempSong->next;
            ++index;
        }
        // Skip the removed song
        prevSong->next = tempSong->next;
        freeSongList(tempSong, 1);
    }
    --playlist->songsNum;
    printf("Song deleted successfully.\n");
}

// Playlist actions
void watchPlaylist(Playlist *playlist) {
    printPlaylist(playlist);

    // Assuming input is valid
    int choice = -1, consumedChar;
    while (choice != SONG_QUIT) {
        printf("choose a song to play, or 0 to quit:\n");

        scanf(" %d", &choice);
        consumedChar = getchar(); // Consume \r\n
        if (consumedChar == '\r') {
            getchar();
        }
        if (choice == SONG_QUIT) {
            return;
        }

        SongList *songNode = findSongByIndex(playlist->songList, choice);
        playSong(songNode->song);
    }
}

void playlistMenu(Playlist *playlist) {
    printf("playlist %s:\n", playlist->name);

    // Init with invalid value
    char choice = '0';
    int consumedChar;
    while (choice != PLAYLIST_EXIT) {
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
        scanf(" %c", &choice);
        consumedChar = getchar(); // Consume \r\n
        if (consumedChar == '\r') {
            getchar();
        }

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

void watchPlaylists(PlaylistList *playlists, int numOfPlaylists) {
    int choice = 0, exitOption = numOfPlaylists + 1, consumedChar;
    while (choice != exitOption + 1) {
        printPlaylists(playlists, numOfPlaylists);

        scanf(" %d", &choice);
        while (choice < 0 || choice > exitOption) {
            scanf(" %d", &choice);
        }
        // Consume \n

        consumedChar = getchar();
        if (consumedChar == '\r') {
            getchar();
        }
        if (choice == exitOption) {
            return;
        }
        PlaylistList *playlistNode = findPlaylistByIndex(playlists, choice);
        playlistMenu(playlistNode->playlist);
    }
}

void addPlaylist(PlaylistList *playlists, int *numOfPlaylists) {
    printf("Enter playlist's name:\n");

    // Start with a string in the len of 1
    char *playlistName = getString();

    // Create new playlist.
    Playlist *newPlaylist = malloc(sizeof(Playlist));
    newPlaylist->name = playlistName;
    newPlaylist->songsNum = 0;
    newPlaylist->songList = calloc(1, sizeof(SongList));
    if (newPlaylist->songList == NULL) {
        printf("memory allocation failed\n");
        exit(ERROR_EXIT_CODE);
    }

    PlaylistList *lastPlaylist;

    if (playlists->playlist == NULL)
        playlists->playlist = newPlaylist;
    else {
        lastPlaylist = findLastPlaylist(playlists, *numOfPlaylists);
        PlaylistList *newNode = calloc(1, sizeof(PlaylistList));
        if (newNode == NULL) {
            printf("memory allocation failed\n");
            exit(ERROR_EXIT_CODE);
        }
        newNode->playlist = newPlaylist;
        newNode->next = NULL;

        // Add node to playlists
        lastPlaylist->next = newNode;
    }
    ++*numOfPlaylists;
}

PlaylistList *removePlaylist(PlaylistList *playlists, int *numOfPlaylists) {
    // Print menu
    printPlaylists(playlists, *numOfPlaylists);

    int choice, lastChoice = *numOfPlaylists + 1, consumedChar;
    scanf(" %d", &choice);
    while (choice < 0 || choice > lastChoice) {
        printf("Invalid Choice\n");
        scanf(" %d", &choice);
        consumedChar = getchar(); // Consume \r\n
        if (consumedChar == '\r') {
            getchar();
        }
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
        freePlaylists(tempPlaylist, 1);
    } else {
        int index = 2;
        PlaylistList *prevPlaylist = playlists;
        tempPlaylist = prevPlaylist->next;

        // Find index of playlist inputted by user
        while (index < choice) {
            prevPlaylist = tempPlaylist;
            tempPlaylist = tempPlaylist->next;
            ++index;
        }
        // Skip the removed song
        prevPlaylist->next = tempPlaylist->next;
        freePlaylists(tempPlaylist, 1);
    }
    if (playlists == NULL) {
        playlists = calloc(1, sizeof(PlaylistList));
        if (playlists == NULL) {
            printf("memory allocation failed\n");
            exit(ERROR_EXIT_CODE);
        }
        playlists->playlist = NULL;
    }
    printf("Playlist deleted.\n");
    --*numOfPlaylists;
    return playlists;
}
