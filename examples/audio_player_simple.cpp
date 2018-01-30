#include <nana/audio/player.hpp>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "1st arg (wav file) is required.\n");
        return 1;
    }
    
    nana::audio::player player(argv[1]);
    player.play();  //Play the audio, it waits until the audio is finished.
    
    return 0;
}

