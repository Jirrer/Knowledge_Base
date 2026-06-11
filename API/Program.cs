

using System.Data.Common;
using SQLitePCL;

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

app.MapGet("/Pull-Data", (HttpContext context) =>
{
    var userKey = context.Request.Headers["key"].ToString();

    if (!database.ValidUser(userKey))
    {
        return Results.Unauthorized();
    }

    List<Database_Entry> databaseOutput = database.PullData(userKey);

    return Results.Ok(databaseOutput);

});

app.MapPost("/Push-Data", (HttpContext context, List<Database_Entry> entries) =>
{
    var userKey = context.Request.Headers["key"].ToString();

    if (!database.ValidUser(userKey))
    {
        return Results.Unauthorized();
    }

    database.InsertIntoData(userKey, entries);
    

    return Results.Created();
});

app.Run();

record Database_Entry(int Id, string Title, string Category, string? Content);