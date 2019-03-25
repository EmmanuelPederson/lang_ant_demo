import setuptools

lang_ant_ext = setuptools.Extension(
    'lang_ant', 
    sources=[
        'lang_ant/src/lang_ant_module.c', 
        'lang_ant/src/lang_ant.c', 
        'lang_ant/src/dict_lang_rule.c',
        'lang_ant/src/pos_hash.c'
    ],
    library_dirs=['/usr/mannynuel/lang_ant/lang_ant/src/pos_hash']
)

setuptools.setup(
    name='LangAnt',
    version='1.0',
    ext_modules=[lang_ant_ext],
    summary="Run simulation following the rules of Langton's Ant",
    description="""Langton's Ant describes a 2D simulation of an ant's
 movement with the following rules: 1) If the current
 cell is in state A, then change the cell state to B and
 turn direction to the left. 2) If the current cell is in
 state B, then change the cell state to A and turn right.
 3) Move foward one step in the direction the ant is
 facing.""",
    author='Emmanuel Pederson',
    author_email='david.pederson@my.wheaton.edu',
)
