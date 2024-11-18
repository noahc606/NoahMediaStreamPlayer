# NoahMediaStreamPlayer
Noah's experimental cross-platform video/audio player. This is NOT YET practical software.
Depends on <a href="https://github.com/noahc606/NCH-CPP-Utils">NCH-CPP-Utils</a> which must be within the 'include'. These are the modules required: "cpp-utils", "sdl-utils", and "ffmpeg-utils".

The video player loops the first valid video it can find within the folder it was executed ('bin/').
Also, do not play long or high-resolution videos - there is not any caching system implemented yet and so it tries to decode the entire video at once (can take up gigabytes of RAM).
