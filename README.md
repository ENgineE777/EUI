# EUI

This is a cross-platform GUI Library, currently only Windows supported. Windows realization have
two imlementaions. First one are using raw Winapi that is why this implemntation are very fast with low
memory footprint. Second implmentation is completely stand alone implementation with rendering
based on DX11. Both implementation have skin system and layout system based on horizontal and verticcal
dividers.

Currently library support follow controls:
- button
- vertical list with categories (needed for editors)
- checkbox
- edit box
- vettical & horizontal linear layout
- label
- listbox
- menu/popup
- panel
- vertical & horizontal scroolbar
- tab/tab sheet
- treeview
- window
- wrapped File dialog
- wrapped Color dialog