# This file contains the default trip point settings and color settings for the solar panel monitor.

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

MAIN_BACKGROUND_COLOR = '#86a3bf' # '#383735' # MID_GRAY_2
# this color highlights the space between and around the main widgets
# '#1a5276' - dark blue background
# '#86a3bf' - light blue background
# '#d9d9d9' - gray background
# '#797b8f' - blue-gray background

WIDGET_BACKGROUND_COLOR = '#d5e5f0' #LIGHT_GRAY
# this color fills the background of all widgets

GRAPH_BACKGROUND_COLOR = '#ffffff' # '#262523' # DARK_GRAY
# this color is the background of the graph

INACTIVE_BUTTON_COLOR = '#ffffff' # WHITE
# this color fills the background of interactive components when they are not being interacted with

ACTIVE_BUTTON_COLOR = '#d9d9d9' # '#cccccc' # GREEN
# this color fills the background of interactive components when they are being interacted with

TEXT_COLOR = '#000000' # BLACK
# this color highlights text and the borders of the widgets

CONNECTOR_WIDGET_COLOR = '#f011b8' # '#464646' # MID_GRAY_1 
# I don't have a clue what this highlights. Changing the color doesn't seem to change anything on the GUI
# because I don't know what this highlights, I have it currently set to hot pink. This way, I will notice if any instance where it is used occurs.

RED = '#a83232'

GREEN = '#457554'

