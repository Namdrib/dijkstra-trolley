# dijkstra-trolley

## About
Runs Dijkstra's algorithm (or BFS) on a bidirectional graph, given in a file of the following format:

```
n
a1 b1 c1
a2 b2 c2
...
am bm cm
```

where:
- `n` : how many nodes in the map
- `ai bi ci` : node ai has a bidirectional path to node bi with cost ci
	- that is, `a1 b1 c1` implies `b1 a1 c1`

- Either input from stdin, or provide a file using the `-f filename` switch when running `./main`

## Requirements
- g++11
- make (optional)

## Running instructions
### Compiling
`make all` creates an executable `main`

### Running
```
Usage: ./main [options]

Options:
  -f filename           Read input from `filename`. (default: read from stdin)
  -h                    Show this help message and exit program
  -i number             Set entry node to node `number` (default: 1)
  -o number             Set target node to node `number` (default: 30)
  -s {BFS,DIJKSTRA}     Set search strategy to use `strat` (default: DIJKSTRA)
```
