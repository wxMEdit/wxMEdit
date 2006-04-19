Welcome to wxAUI 0.9.2


Overview
--------

wxAUI is an Advanced User Interface library for the wxWidgets toolkit that allows developers to create high-quality, cross-platform user interfaces quickly and easily.


Features
--------

With wxAUI version 0.9.2, developers can create application frameworks with:

    * Native, dockable floating frames
    * Perspective saving and loading (similar to Eclipse's perspectives)
    * Native toolbars incorporating real-time, "spring-loaded" dragging
    * Customizable floating/docking behavior
    * Completely customizable look-and-feel
    * Optional transparent window effects (while dragging or docking)

What's new in 0.9.2?

    * Support for wxMac
    * Updates for wxWidgets 2.6.3
    * Fix to pass more unused events through
    * Fix to allow floating windows to receive idle events
    * Fix for minimizing/maximizing problem with transparent hint pane
    * Fix to not paint empty hint rectangles
    * Fix for 64-bit compilation

What's new in 0.9.1?

    * Support for MDI frames
    * Gradient captions option
    * Active/Inactive panes option
    * Fix for screen artifacts/paint problems
    * Fix for hiding/showing floated window problem
    * Fix for floating pane sizing problem
    * Fix for drop position problem when dragging around center pane margins
    * LF-only text file formatting for source code


Known Problems
--------------

    * Pane Minimum size is not quite finished
    * Pane Maximum size still needs to be written
    * Clunky redocking still exists in wxGTK version


Upcoming Features
-----------------

We have many ideas for upcoming versions.  Some of our ideas include the following:

    * Better toolbar support
    * A so-called "Chevron" button on toolbars to make them expandable
    * Optional HUD-style docking (similar to VC++ 2005's docking)
    * Separate border size for toolbars
    * Dock collapse button (as seen in OOo 2.0)
    * Minimize/Maximize/Pin pane buttons
    * Reduce flicker and artifacts to 0%

