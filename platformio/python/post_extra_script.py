Import("env", "projenv")

try:
    import pydantic
except ImportError:
    env.Execute("$PYTHONEXE -m pip install pydantic")

from configschema import ConfigSchema, defined_enums
import json
from re import sub

def upper_snake(s: str):
  return '_'.join(
    sub('([A-Z][a-z]+)', r' \1',
    sub('([A-Z]+)', r' \1', s)).split()).upper()

cppDefines = {}

with open("./config.json", "r", encoding="utf-8") as config_file:
    user_config = json.load(config_file)
    config = ConfigSchema(**user_config)

    # Add enum members as macros
    for enum in defined_enums:
        for i, member in enumerate(enum):
            cppDefines[member.name] = i

    # Font name to header mappings
    font_files = {
        "FreeMono": "fonts/FreeMono.h",
        "FreeSans": "fonts/FreeSans.h",
        "FreeSerif": "fonts/FreeSerif.h",
        "Lato": "fonts/Lato_Regular.h",
        "Montserrat": "fonts/Montserrat_Regular.h",
        "Open Sans": "fonts/OpenSans_Regular.h",
        "Poppins": "fonts/Poppins_Regular.h",
        "Quicksand": "fonts/Quicksand_Regular.h",
        "Raleway": "fonts/Raleway_Regular.h",
        "Roboto": "fonts/Roboto_Regular.h",
        "Roboto Mono": "fonts/RobotoMono_Regular.h",
        "Roboto Slab": "fonts/RobotoSlab_Regular.h",
        "Ubuntu": "fonts/Ubuntu_R.h",
        "Ubuntu Mono": "fonts/UbuntuMono_R.h"
    }

    for k,v in config:
        if hasattr(v, "name"):
            if k == "locale":
                # For locale take its value and add to macro as literal
                cppDefines[upper_snake(k)] = v.value
            elif k == "font":
                # Font is taken from the font_files dictionary as string
                cppDefines["FONT_HEADER"] = env.StringifyMacro(font_files[v])
            else:
                # For the other enums take the member name (not its value)
                cppDefines[upper_snake(k)] = v.name
        elif type(v) == str:
            cppDefines["D_" + upper_snake(k)] = env.StringifyMacro(v)
        elif v is None:
            cppDefines["D_" + upper_snake(k)] = env.StringifyMacro("")
        elif type(v) == bool:
            cppDefines[upper_snake(k)] = int(v)
        else:
            cppDefines[upper_snake(k)] = v

    print("Defines:")
    print(json.dumps(cppDefines, indent=4, ensure_ascii=False))

    projenv.Append(CPPDEFINES=cppDefines)



"""
# https://docs.platformio.org/en/latest/scripting/examples/asking_for_input.html
# TODO: To get the input (e.g. Wi-Fi password):
print("Enter your name:")
user_name = input()

# https://docs.platformio.org/en/latest/scripting/actions.html
env.AddPreAction("buildprog", callback...)
"""