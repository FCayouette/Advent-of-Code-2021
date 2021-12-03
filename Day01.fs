open System.IO

[<EntryPoint>]
let main argv =
    let data = File.ReadAllLines(argv.[0]) |> Seq.map System.Int32.Parse
    Seq.pairwise data |> Seq.filter(fun (a,b) -> a < b) |> Seq.length |> printfn "Part 1: %A"
    Seq.windowed 4 data |> Seq.filter(fun a -> a.[0] < a.[3]) |> Seq.length |> printfn "Part 2: %A"
    0
