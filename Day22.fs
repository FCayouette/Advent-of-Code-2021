open System.IO

let Process (s:string) =
    let data = s.Split(' ')
    let simple = data.[1].Replace("..", ".").Replace("x=", "").Replace(",y=", ".").Replace(",z=",".")
    let bounds = simple.Split('.') |> Array.map System.Int64.Parse
    (data.[0].[1]='n', (bounds.[0], bounds.[1], bounds.[2], bounds.[3], bounds.[4], bounds.[5]))

let IsInit (_,(x,X,y,Y,z,Z)) = 
    (Seq.filter(fun x -> 50L < abs x) [x;X;y;Y;z;Z] |> Seq.length)=0

let Intersects (x0,X0,y0,Y0,z0,Z0) (x1,X1,y1,Y1,z1,Z1) =
    not (X0<x1 || X1<x0 || Y0<y1 || Y1<y0 || Z0<z1 || Z1<z0)

let Intersection (x0,X0,y0,Y0,z0,Z0) (x1,X1,y1,Y1,z1,Z1) =
    let x = Array.sort [|x0; X0; x1; X1|]
    let y = Array.sort [|y0; Y0; y1; Y1|]
    let z = Array.sort [|z0; Z0; z1; Z1|];
    (x.[1], x.[2], y.[1], y.[2], z.[1], z.[2])

let Volume (x,X,y,Y,z,Z) = (X-x+1L)*(Y-y+1L)*(Z-z+1L)

let Activate (inst:(bool*'a)[]) =
    let mutable toAdd, toRemove = [||], [||]
    Array.iter(fun (on, x) ->
        let mutable work = [||]
        let AppendWork x = work<-Array.append work [|x|]
        if on then
            Array.iter(fun y -> if Intersects x y then AppendWork (Intersection x y)) toAdd
            toAdd<-Array.append toAdd [|x|]
            Array.iter(fun y -> if Intersects x y then toAdd <- Array.append toAdd [|Intersection x y|]) toRemove
            toRemove<-Array.append toRemove work                
        else
            Array.iter(fun y -> if Intersects x y then AppendWork (Intersection x y)) toRemove
            Array.iter(fun y -> if Intersects x y then toRemove <- Array.append toRemove [|Intersection x y|]) toAdd
            toAdd<-Array.append toAdd work) inst
    (Array.map(Volume) toAdd |> Array.sum) - (Array.map(Volume) toRemove |> Array.sum)

[<EntryPoint>]
let main argv =
    let instructions = File.ReadAllLines(argv.[0]) |> Array.map Process
    printfn "Part 1: %d" <| Activate (Array.filter(IsInit) instructions)
    printfn "Part 2: %d" <| Activate instructions
    0
