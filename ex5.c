#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int numOfPlaylist;
    struct PlaylistList *next;
} PlaylistList;

char *getString() {
    char *string = calloc(1, sizeof(char));
    // Get a single char each time
    char newChar = (char) getchar();
    int counter = 0;
    while (newChar != '\n') {
        // Add new char to current index
        string[counter] = newChar;
        string[counter + 1] = '\0';
        ++counter;

        // Resize the string to have space for the new char
        string = realloc(string, sizeof(string) + sizeof(char));

        // If realloc failed, crash
        if (string == NULL) {
            free(string);
            exit(1);
        }

        // read the next char
        newChar = (char) getchar();
    }
    return string;
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

void watchPlayList(Playlist *playlist) {
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
        lastNode->next = newNode;
    } else {
        // If current node is empty, this is the node we are editing.
        newNode = lastNode;
    }
    // Create emtpy song
    newNode->song = calloc(1, sizeof(Song));

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
                watchPlayList(playlist);
                printf("Exited");
                break;
            }
            case ADD_SONG: {
                addSong(playlist);
                break;
            }
            case PLAY_SONG: {
                playAllSongs(playlist);
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

void watchPlaylists(PlaylistList *playlists) {
    int lastChoice = 1;

    // Point to the first playlist
    PlaylistList *currentPlaylist = playlists;
    for (int i = 0; i < playlists->numOfPlaylist; ++i) {
        // Print the name of current playlist, advance pointer.
        printf("%d. %s\n", lastChoice, currentPlaylist->playlist->name);
        currentPlaylist = currentPlaylist->next;
        ++lastChoice;
    }
    printf("%d. Back to main menu\n", lastChoice);
    int choice;
    scanf(" %d", &choice);
    while (choice < 0 || choice > lastChoice) {
        printf("Invalid Choice\n");
        scanf(" %d", &choice);
    }
    // Consume \n
    getchar();
    if (choice == lastChoice) {
        return;
    }
    PlaylistList *playlistNode = findPlaylistByIndex(playlists, choice);
    playlistMenu(playlistNode->playlist);
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
    char *playlistName = getString();

    // Create new playlist.
    Playlist *newPlaylist = malloc(sizeof(PlaylistList));
    newPlaylist->name = playlistName;
    newPlaylist->songsNum = 0;
    newPlaylist->songList = calloc(1, sizeof(SongList));

    PlaylistList *lastPlaylist;

    if (playlists->playlist == NULL)

        lastPlaylist = playlists;
    else {
        lastPlaylist = findLastPlaylist(playlists);
        lastPlaylist = lastPlaylist->next;
    }

    // Add node to playlists
    lastPlaylist->playlist = newPlaylist;
    ++playlists->numOfPlaylist;
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

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

int main() {
    // Set the default value of choice to a null one.
    char choice = '0';
    // Song *song = calloc(1, sizeof(0));
    // SongList *songList = calloc(1, sizeof(songList));
    // songList->song = song;
    // Playlist *p = calloc(1, sizeof(Playlist));
    // p->songList = songList;
    PlaylistList *playlists = calloc(1, sizeof(PlaylistList));
    if (playlists == NULL) {
        return 1;
    }
    // playlists->playlist = p;
    // printf("%s\n", p->songList->song->artist);
    // Initialize the playlists' list.
    playlists->numOfPlaylist = 0;

    while (choice != MAIN_EXIT) {
        printPlaylistsMenu();
        scanf(" %c", &choice);
        switch (choice) {
            case WATCH_PLAYLIST: {
                watchPlaylists(playlists);
                break;
            }
            case ADD_PLAYLIST: {
                AddPlaylist(playlists);
                // printf("%s\n", playlists->playlist->name);
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
    printf("Goodbye!\n");
}
