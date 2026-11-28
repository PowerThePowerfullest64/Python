# This example requires the 'message_content' intent.
import discord
import asyncio
import Femboy
import re

mention_labels = [
    "zack",
    "catboy",
    "gayboy",
    "femboy",
    "ladyboy",
    "fem",
    "boy",
    "gay",
    "nigger",
    "sack",
    "zuck",
    "zucker",
    "zacky",
    "cat",
    "kitten",
    "kitty",
    "miaw",
    "meow",
    "neger",
    "nigga",
    "nga",
    "zawg",
    "dawg",
]

presence = "Feelin' extra fuzzy today! 😽✨"

# 1. Reusable member lookup helper.
def get_member(guild: discord.Guild, alias: str) -> discord.Member | None:
    alias = alias.strip().lstrip("@")
    
    # Exact match
    member = guild.get_member_named(alias)
    if member:
        return member

    # ID or <@123456> mention match.
    id_match = re.search(r"\d{15,20}", alias)
    if id_match:
        member = guild.get_member(int(id_match.group(0)))
        if member:
            return member

    # Case-insensitive fallback across username, display name, and nickname.
    member = guild.get_member_named(alias.lower())
    if member:
        return member
            
    return None

async def cmd_kick(args: list[str], message: discord.Message):
    if not message.guild or not args:
        return
    
    target = get_member(message.guild, args[0])
    reason = args[1] if len(args) > 1 and args[1] else "No reason provided"

    # Check if member was found and if not self.
    if not target or target == message.guild.me:
        return

    try:
        await target.kick(reason=reason)
    except discord.Forbidden:
        print(f"Warning: Tried to kick someone with a higher permission of name '{target.name}'.")

async def cmd_nick(args: list[str], message: discord.Message):
    if not message.guild or not args:
        return
    
    # Supports [NICK: new_name] (self) OR [NICK: target, new_name]
    if len(args) == 1:
        target = message.guild.me
        new_nick = args[0]
    else:
        target = get_member(message.guild, args[0])
        new_nick = args[1]

    if not target:
        return

    try:
        await target.edit(nick=new_nick[:32] or None)
    except discord.Forbidden:
        print(f"Warning: Tried to change nickname of someone with a higher permission of name'{target.name}'.")

COMMANDS = {
    "KICK": cmd_kick,
    "NICK": cmd_nick
}

async def do_commands(reply: str, message: discord.Message) -> str:
    matches = re.finditer(r"\[([A-Z]+):\s*(.*?)\]", reply, re.IGNORECASE)

    for match in matches:
        cmd_name = match.group(1).upper()
        raw_args = match.group(2)

        handler = COMMANDS.get(cmd_name)
        if handler:
            # Split args by comma and strip whitespace.
            args = [a.strip() for a in raw_args.split(",")]
            await handler(args, message)

    pattern = r"\[(?:" + "|".join(COMMANDS.keys()) + r"):.*?\]"
    clean_reply = re.sub(pattern, "", reply, flags=re.IGNORECASE).strip()
    return clean_reply
    
        
with open("token.txt", "r") as t:
    TOKEN = t.read()

intents = discord.Intents.default()
intents.message_content = True
#intents.members = True # Andreas needs to fix this!
client = discord.Client(intents=intents)

@client.event
async def on_ready():
    print(f'{client.user} is connected to the following server:\n')
    for server in client.guilds:
        print(f'{server.name} (id: {server.id})')

    await client.change_presence(
        activity=discord.Activity(type=discord.ActivityType.listening, name=presence)
    )

@client.event
async def on_message(message: discord.Message):
    if message.author == client.user:
        return

    if message.author.bot: # Prevent bot back-and-fourth.
        return
    
    clean_content = message.content
    is_mentioned = False
    
    for label in mention_labels:
        if label.lower() in message.content.lower():
            is_mentioned = True
            break

    if client.user in message.mentions:
        is_mentioned = True
        clean_content = (
            clean_content.replace(f"<@{client.user.id}>", "")
            .replace(f"<@!{client.user.id}>", "")
        )

    if clean_content.startswith("!zack"):
        clean_content = clean_content.removeprefix("!zack")
        is_mentioned = True

    clean_content = clean_content.strip()

    if not is_mentioned:
        # Save message to history even though zack wasn't mentioned.
        Femboy.save_user_history(clean_content, message.author.name)
        return
    
    async with message.channel.typing():
        if clean_content == "history":
            history = "This is the chat history that I have recorded, (*nya*):\n"
            for i, msg in enumerate(Femboy.messages):
                if i == 0: # Skip system prompt.
                    continue
                history += msg["content"] + "\n"
            await message.channel.send(history)
            return

        print("-----------------------------------------------------------")
        print(clean_content)

        if message.reference and message.reference.message_id:
            try:
                replied_msg = await message.channel.fetch_message(message.reference.message_id)
                clean_content = f"[Replying to {replied_msg.author.name}: \"{replied_msg.content}\"]\n{clean_content}"
            except discord.NotFound:
                pass
        
        raw_reply = await asyncio.to_thread(Femboy.do_chat, clean_content, message.author.name)
        print(raw_reply)
        clean_reply = await do_commands(raw_reply, message)
        clean_reply = clean_reply[:2000] # Cut off any characters outside 2000 char limit.
        if clean_reply:
            await message.reply(clean_reply)
    

client.run(TOKEN)