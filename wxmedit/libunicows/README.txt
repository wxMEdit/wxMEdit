The versions of libunicow.a and unicows.lib are 1.1.1.
I changed GetTextExtentW() symbal name to GGGTextExtentW(),
and ExtTextOutW() to EeeTextOutW(), 
because they are not work under win98, 
and the native calls of GetTextExtentW() and ExtTextOutW() functions are work fine.
