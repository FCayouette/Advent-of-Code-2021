open System.IO

// Define a function to construct a message to print
let PadMap (x:int[][]) =
    let fullPad = Array.create(x.[0].Length+2) 10
    let result = Array.append [|fullPad|] (Array.map(fun r -> Array.append (Array.append [|10|] r) [|10|]) x)
    Array.append result [|fullPad|]

let lowPoint x y (map:int[][]) = 
    map.[x].[y] < map.[x-1].[y] && map.[x].[y] < map.[x+1].[y] && map.[x].[y] <map.[x].[y-1] && map.[x].[y] <map.[x].[y+1]

let rec BasinFind (x,y) (map:int[][]) seenPos = 
    if map.[x].[y] >= 10 || Array.exists(fun a -> a=(x,y)) seenPos then seenPos
    else BasinFind (x+1, y) map (BasinFind (x-1, y) map (BasinFind (x,y+1) map (BasinFind (x,y-1) map (Array.append seenPos [|(x,y)|]))))

[<EntryPoint>]
let main argv =
    let map = File.ReadAllLines(argv.[0]) |> Array.map(fun (x:string)-> x.ToCharArray() |> Array.map(fun x -> int(x) - int('0') + 1)) |> PadMap
    let x = [| for i in 1 .. map.Length-1 -> i|]
    let y = [| for i in 1 .. map.[0].Length - 1 -> i|]
    let lowPoints = Array.fold(fun points i -> Array.append points (Array.filter(fun y -> lowPoint i y map) y |> Array.map(fun y -> (i, y)))) [||] x
    Array.fold(fun total (i, j)-> total + map.[i].[j]) 0 lowPoints |> printfn "Part 1: %A"
    let basinSizes = Array.map(fun x -> BasinFind x map [||] |> Array.length) lowPoints |> Array.sortDescending
    Array.fold(*) 1 basinSizes.[..2] |> printfn "Part 2: %A"
    0 
