open System.IO

[<EntryPoint>]
let main argv =
    let area = File.ReadAllText(argv.[0]).Replace("target area: x=", "").Replace(" y=", "").Replace("..", ",").Split(',') |> Array.map System.Int32.Parse
    let (tx, tX, ty, tY) = (area.[0], area.[1], area.[2], area.[3])
    printfn "Part 1: %i" (ty * (ty+1) /2)

    let rec Simulate (px, py) (vx,vy) =
        if px > tX || py < ty then 0 
        elif px >= tx && py <= tY then 1 else
        Simulate (px+vx, py+vy) ((max 0 (vx-1)),vy-1)

    let Sum x = Array.sum [|for y in ty .. -ty -> Simulate (0,0) (x,y)|]
    printfn "Part 2: %i" <| Array.fold(fun total x -> total + (Sum x)) 0 [|0 .. tX|]
    0