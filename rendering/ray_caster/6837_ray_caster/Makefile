INCFLAGS  = -I /mit/6.837/public/include/vecmath
LINKFLAGS = -L /mit/6.837/public/lib -lglut -lvecmath
CFLAGS    = -g -Wall -Wextra -Wno-unused -Wno-unused-parameter -ansi -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC #debugging compiler commands
CC = g++
SRCS = main.cpp BoundingBox.cpp BoundingSphere.cpp Camera.cpp Checkerboard.cpp DirectionalLight.cpp Film.cpp Filter.cpp Grid.cpp Group.cpp Image.cpp Light.cpp Marble.cpp Material.cpp Noise.cpp Object3D.cpp PerlinNoise.cpp Plane.cpp PointLight.cpp RayTracer.cpp Sample.cpp SampleDebugger.cpp Sampler.cpp SceneParser.cpp Sphere.cpp Transform.cpp Triangle.cpp Wood.cpp

OBJS = $(SRCS:.cpp=.o)
PROG = a5

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

clean:
	rm -f *.bak *.o core.* $(PROG) 
