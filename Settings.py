# This file contains the default trip point settings and color settings for the solar panel monitor.
# The original project owners called this file "Globs.py", but it was renamed to better reflect the file's purpose.

    # -------- Trip Points -------- #

DEFAULT_VOLTAGE_TRIP_POINT = 100.0
DEFAULT_CURRENT_TRIP_POINT = 10.0
DEFAULT_TEMPERATURE_TRIP_POINT = 85.0

MAX_VOLTAGE_TRIP_POINT = 100.0
MAX_CURRENT_TRIP_POINT = 10.0
MAX_TEMPERATURE_TRIP_POINT = 85.0

MIN_VOLTAGE_TRIP_POINT = 0.0
MIN_CURRENT_TRIP_POINT = 0.0
MIN_TEMPERATURE_TRIP_POINT = -40.0

    # ---------- Colors ---------- #

MAIN_BACKGROUND_COLOR = '#1a5276' # '#383735' # MID_GRAY_2
# this color highlights the space between and around the main widgets

CONNECTOR_WIDGET_COLOR = '#464646' # MID_GRAY_1 
# I don't have a clue what this highlights. Changing the color doesn't seem to change anything on the GUI

DATA_WIDGET_COLOR = '#ababab' #LIGHT_GRAY
# this color fills the background of the data widget

INACTIVE_BUTTON_COLOR = '#ffffff' # WHITE
# this color fills of background of interactive components when they are not being pressed

ACTIVE_BUTTON_COLOR = '#d9d9d9' # '#cccccc' # GREEN
# this color highlights interactive components when they are interacted with

FIFTY_SHADES_OF_GRAY = '#262523' # DARK_GRAY
# this color fills the background of the connector widget and is the background of the graph

TEXT_COLOR = '#000000' # BLACK
# this color highlights text and the borders of the widgets

RED = '#ff6262'

GREEN = '#cccccc'

