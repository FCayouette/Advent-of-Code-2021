open System.IO

let LinDiff i lobsters =
    Array.fold(fun t x -> t + abs(x-i)) 0L lobsters

let QuadDiff i lobsters =
    Array.fold(fun t x -> t + (abs(x-i) * (abs(x-i)+1L)/2L)) 0L lobsters

[<EntryPoint>]
let main argv =
    let lobsters = File.ReadAllText(argv.[0]).Split(',') |> Array.map System.Int64.Parse
    let pos = [|(Array.min lobsters) .. (Array.max lobsters)|]
    Array.map(fun x -> LinDiff x lobsters) pos |> Array.min |> printfn "Part 1: %A"
    Array.map(fun x -> QuadDiff x lobsters) pos |> Array.min |> printfn "Part 2: %A"
    0
