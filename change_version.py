import sys
import json

def update_engine_version(version):
    # Open the CronosPlayUnreal.uplugin file and load its contents as a JSON object
    with open('CronosPlayUnreal.uplugin', 'r') as f:
        data = json.load(f)

    # Update the value of EngineVersion to the specified version parameter
    data['EngineVersion'] = version

    # Write the updated JSON object back to the CronosPlayUnreal.uplugin file
    with open('CronosPlayUnreal.uplugin', 'w') as f:
        json.dump(data, f, indent='\t')

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python change_version.py <version>")
    else:
        version = sys.argv[1]
        update_engine_version(version)
