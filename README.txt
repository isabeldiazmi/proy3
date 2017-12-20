Crear un workspace mkdir EK_AutoNOMOS
Crear un paquete AutoNOMOS_simulation

Descargar del repositorio la carpeta "src" que contiene los archivos del programa, y pegarla en el paquete antes mencionado.

Desde la terminal, ejecutar los dos comandos siguientes respectivamente: 

"catkin_make"
"setup.bash source devel/setup.bash"

Parte 1:
El programa desarrollado para la primer parte del proyecto es "aipn2.cpp". 
Esta parte consistió en la estimación del estado por medio de un filtro de histogramas se corre con el primer ejecutable anteriormente mencionado.

Para ejecutarlo, estando en la carpeta de "autonomos_simulation", se debe correr en una terminal el siguiente comando para ejecutar la bag de ros asignada:
"rosbag play rosbag_SDI11911.bag"

En otra terminal, estando en la carpeta "AutoNOMOS_simulation", ejecutar el siguiente comando:
"rosrun autonomos_simulation aipn2"

A partir de esto, se visualizarán los estados en los que se encuentra el robot según la información extraída de los 4 diferentes tópicos.


Parte 2:
El programa desarrollado para la segunda parte del proyecto es "filtroKalman.cpp".

Para ejecutarlo, se requiere, en una terminal correr:
"rosrun autonomos_simulation filtroKalman"
En otra terminall, correr lo siguiente:
"roslaunch autonomos_gazebo straight_road.launch"





