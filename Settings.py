# This file contains the default customizable settings and color settings for the solar panel monitor.
# The original project owners called this file "Globs.py", but it was renamed to better reflect its purpose.

DEFAULT_VOLTAGE_TRIP_POINT = 100.0
DEFAULT_CURRENT_TRIP_POINT = 10.0
DEFAULT_TEMPERATURE_TRIP_POINT = 85.0

MAX_VOLTAGE_TRIP_POINT = 100.0
MAX_CURRENT_TRIP_POINT = 10.0
MAX_TEMPERATURE_TRIP_POINT = 85.0

MIN_VOLTAGE_TRIP_POINT = 0.0
MIN_CURRENT_TRIP_POINT = 0.0
MIN_TEMPERATURE_TRIP_POINT = -40.0

WHITE = '#ffffff' # this color highlights buttons when they are not being pressed
LIGHT_GRAY = '#ababab' # this color highlights the data widget and should not be changed
MID_GRAY_1 = '#464646' # this color highlights the connection widget
MID_GRAY_2 = '#383735' # this color highlights the space between and around the main widgets
DARK_GRAY = '#262523'
BLACK = '#000000' # this color highlights text
RED = '#ff6262'
GREEN = WHITE  # '#32cd32' # this color highlights interactive components when they are interacted with