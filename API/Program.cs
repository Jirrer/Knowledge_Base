

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
// Learn more about configuring OpenAPI at https://aka.ms/aspnet/openapi
builder.Services.AddOpenApi();

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.MapOpenApi();
}

app.UseHttpsRedirection();

app.MapGet("/Ping-Server", () => "Success");

Database database = new();

HashSet<string> keys = new HashSet<string> {"devkey"};

app.MapGet("Pull-Data", (HttpContext context) =>
{
    var userKey = context.Request.Headers["key"].ToString();

    if (!validateKey(userKey))
    {
        return Results.Unauthorized();
    }

    List<Database_Entry> databaseOutput = database.PullData(userKey);

    return Results.Ok(databaseOutput);

});

app.Run();


bool validateKey(string potentialKey)
{
    return keys.Contains(potentialKey);
}

record Database_Entry(int Id, string Title, string Category, string? Content);