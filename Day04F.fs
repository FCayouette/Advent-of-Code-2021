open System.IO

type Grid(d: int[][]) =
    let mutable grid = Array.map(fun x -> Array.map(fun a -> (a, false)) x) d
    
    member this.GetScore = Array.fold(fun t x -> t + Array.fold(fun t (v, f) -> if f then t else t+v) 0 x) 0 grid
    member this.AddVal x = 
        for i in [|0 .. grid.Length-1|] do
           grid.[i] <- Array.map(fun (i,b) -> if i=x then (i, true) else (i,b)) grid.[i]
    member this.HasBingo =
        let h = Array.fold(fun t a-> t || Array.fold(fun c (_,v) -> (c && v)) true a) false grid
        h || Array.fold(fun t line -> Array.map2(fun b (_,v) -> b&&v) t line) (Array.create(grid.[0].Length) true) grid |> Array.fold(||) false 
        
let rec ProcessGrids (nums:int[]) (grids:Grid[]) (scores:int[]) =
    if grids.Length = 0 then scores
    else
        let curNum = nums.[0]
        Array.iter(fun (x:Grid) -> x.AddVal curNum) grids
        let (bingo, toProcess) = Array.partition(fun (x:Grid) -> x.HasBingo) grids
        let newScores = Array.map(fun (g:Grid) -> curNum * g.GetScore) bingo
        ProcessGrids nums.[1..] toProcess (Array.append scores newScores)

[<EntryPoint>]
let main argv =
    let a = File.ReadAllLines(argv.[0])
    let numbers = a.[0].Split(',') |> Array.map System.Int32.Parse
    let rawGrids = a.[1..] |> Array.filter(fun (x:string) -> x.Length > 0)
    let Grids = Array.map(fun (x:string) -> x.Split(' ') |> Array.filter(fun (x:string)->x.Length>0) |> Array.map System.Int32.Parse) rawGrids |> Array.chunkBySize 5 |> Array.map Grid
    let scores = ProcessGrids numbers Grids [||]
    printfn "Part 1 %A\nPart 2 %A" scores.[0] scores.[scores.Length-1]
    0 // return an integer exit code
