open System.IO

let StringToPoint (s:string) =
    let xy = s.Split(',') |> Array.map(fun (x:string) -> System.Int32.Parse x)
    (xy.[0], xy.[1])
    
let StringToInst (s:string) =
    let inst = s.Replace("fold along ", "").Split('=')
    (inst.[0].[0]='x', System.Int32.Parse inst.[1])

let ApplyInstruction points (onX, fold) =
    Array.map(fun (x,y) -> if onX && x > fold then (2*fold-x, y) elif not onX && y > fold then (x, 2*fold-y) else (x,y)) points |> Array.distinct
    
let PrintPoints points =
    let (maxX,_) = Array.maxBy(fst) points
    let (_, maxY) = Array.maxBy(snd) points
    for y in 0 .. maxY do
        Array.fold(fun (s:string) x -> s + (if None<>Array.tryFind(fun p-> p=(x,y)) points then "#" else " ")) "" [|0..maxX|] |> printfn("%s")

[<EntryPoint>]
let main argv =
    let raw = File.ReadAllLines(argv.[0])
    let initialPoints = Array.filter(fun (x:string)->x.Contains(",")) raw |> Array.map(StringToPoint)
    let instructions = Array.filter(fun (x:string)->x.Contains("=")) raw |> Array.map(StringToInst)
    ApplyInstruction initialPoints instructions.[0] |> Array.length |> printfn "Part 1: %A\nPart 2:"
    Array.fold(ApplyInstruction) initialPoints instructions |> PrintPoints 
    0
