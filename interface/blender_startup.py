import os

import bpy

head = os.path.expanduser('~')
os.chdir(os.path.join(head, 'ardu-accelo/interface/'))

for o in bpy.context.scene.objects:
    if o.name == 'Cube':
        box = o

bpy.ops.logic.sensor_add(type="ALWAYS", object=box.name)
bpy.ops.logic.controller_add(type="PYTHON", object=box.name)

python_controller = box.game.controllers[-1]
python_controller.mode = 'MODULE'
python_controller.module = 'server.main'

sensor = box.game.sensors[-1]
sensor.link(python_controller)
# python_controller.link(sensor)
print('started up')
