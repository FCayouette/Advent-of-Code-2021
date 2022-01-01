open System.IO

let Flashes map =
    Array.fold(fun t x -> t + (Array.filter(fun y -> y = 0y) x |> Array.length)) 0 map

let HasFlash map = 
    Array.fold(fun t r -> match t with true -> true | _ -> None <> Array.tryFind(fun x -> x > 9y) r ) false map

let ProcessFlashes map =
    let maxX = Array.length map - 1
    let maxY = Array.length map.[0] - 1
    for i in [0..maxX] do
        for j in [0..maxY] do
            if map.[i].[j] > 9y then 
                for x in [max 0 (i-1) .. min (i+1) maxX] do
                    for y in [max 0 (j-1) .. min (j+1) maxY] do
                        if map.[x].[y] <> 0y then 
                            map.[x].[y] <- map.[x].[y] + 1y
                map.[i].[j] <- 0y
    map

let IterMap map =
    let mutable newMap = Array.map(fun r -> Array.map(fun x -> x+1y) r) map
    while HasFlash newMap do
        newMap <- ProcessFlashes newMap
    newMap                

let rec CountFlashes iter map count = 
    if iter = 0 then count else
    let newMap = IterMap map
    CountFlashes (iter-1) newMap (count + Flashes newMap)

let rec GetFirstAllFlash map i =
    let Sum = Array.fold(fun t x -> match t with false -> false | _ -> None = Array.tryFind(fun x -> x <> 0y) x) true map
    if Sum then i else GetFirstAllFlash (IterMap map) (i+1)

[<EntryPoint>]
let main argv =
    let initialMap = File.ReadAllLines(argv.[0]) |> Array.map(fun (x:string) -> x.ToCharArray() |> Array.map(fun x -> (sbyte)x-(sbyte)'0'))
    printfn "Part 1: %A" <| CountFlashes 100 initialMap 0
    printfn "Part 2: %A" <| GetFirstAllFlash initialMap 0
    0
